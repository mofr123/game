package com.server.handler.coder;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufInputStream;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.util.AttributeKey;

import java.lang.reflect.Method;
import java.security.NoSuchAlgorithmException;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;

import com.IoPuts;
import com.Request;
import com.Respone;
import com.alibaba.fastjson.JSONObject;
import com.caucho.hessian.io.Hessian2Input;
import com.exception.DeserializeException;
import com.server.ServerContext;
import com.spring.GameBootstrap;
import com.util.Loggers;

public class RequestDecoder extends ByteToMessageDecoder {

	private static Logger logger = Logger.getLogger(RequestDecoder.class);

	@Override
	protected void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception,
			NoSuchAlgorithmException {

		if (in.readableBytes() > 0) {
			if (in.getByte(in.readerIndex()) == Byte.MAX_VALUE) {
				// beat
				in.readByte();
				if (logger.isDebugEnabled()) {
					logBeat(ctx);
				}
				out.add(Request.ping());
				return;
			}
		}

		if(!getDecodeStat(ctx.channel()).OK) {
			while (in.readableBytes() > 0) {
				in.readByte();
			}
		}
		
		if (in.readableBytes() < 5) {
			return;
		}
		if(in.readableBytes() < 5 + in.getUnsignedMedium(in.readerIndex() + 2)) {
			return;
		}
		
		if (in.readableBytes() >= 1 && in.getByte(in.readerIndex()) != 1) {
			DeserializeException de = new DeserializeException("unresolve magic data:" + in.getByte(in.readerIndex()));
			JSONObject lo = Loggers.newLogObj(Loggers.DECODE, de);
			logger.error(lo.toJSONString());
			in.discardReadBytes();
			ctx.channel().close();
			throw de;
		}

		Request request = new Request();
		try {
			in.readByte();
			in.readByte();
			int size = in.readUnsignedMedium();
			if (logger.isDebugEnabled()) {
				logSize(ctx, size);
			}
			
			Hessian2Input hessin = IoPuts.hessian2Input();
			try {
				hessin.resetBuffer();
			} catch (Exception e) {
			}
//			EncryptInputStream eis = new EncryptInputStream(adler32, new ByteBufInputStream(in, size));
//			hessin.init(eis);
			
			hessin.init(new ByteBufInputStream(in, size));

			hessin.startMessage();
			String type = hessin.readString();
			request.setType(type);
			request.setId(hessin.readLong());

			if (type.equals("2")) {
				// pre_rpc
				request.setOrg(hessin.readString());
				request.setApp(hessin.readString());
				request.setProps((Map<String, Object>) hessin.readObject());
			} else if (type.equals("3")) {
				// rpc
				int mid = hessin.readInt();
				if (mid == 0) {
					request.setC(hessin.readString());
					request.setM(hessin.readString());
					request.setArgTypes((String[]) hessin.readObject());
				} else {
					request.setMid(mid);
				}
				request.setArgs((Object[]) hessin.readObject());
			}

			if (logger.isInfoEnabled()) {
				logRpc(ctx, request);
			}
			hessin.completeMessage();

			out.add(request);
			getDecodeStat(ctx.channel()).lastId = request.getId();
		} catch (Exception e) {
			// 反序列化失败
			JSONObject lo = Loggers.newLogObj(Loggers.DECODE_ERR, e);
			lo.put("id", request.getId());
			lo.put("type", request.getType());
			lo.put("from", ctx.channel().remoteAddress());
			logger.error(lo.toJSONString());
			
			Channel channel = ctx.channel();
			DecodeStat decodeStat = getDecodeStat(channel);
			decodeStat.OK = false;
			Respone respone = Respone.decodeFail(decodeStat.lastId);
			ctx.channel().writeAndFlush(respone);
			
			while (in.readableBytes() > 0) {
				in.readByte();
			}
			throw e;
		}
	}

	private void logSize(ChannelHandlerContext ctx, int size) {
		JSONObject lo = Loggers.newLogObj(Loggers.DECODE);
		lo.put("from", ctx.channel().remoteAddress());
		lo.put("size", size);
		logger.debug(lo.toJSONString());
	}

	private void logRpc(ChannelHandlerContext ctx, Request request) {
		JSONObject lo = Loggers.newLogObj(Loggers.REQUEST_IN);
		lo.put("from", ctx.channel().remoteAddress());
		lo.put("id", request.getId());
		lo.put("type", request.getType());

		if (Request.Type.PRE_RPC.equals(request.getType())) {
			if (GameBootstrap.printDetail) {
				lo.put("props", JSONObject.toJSONString(request.getProps()));
			}
		} else if (Request.Type.RPC.equals(request.getType())) {
			lo.put("mid", request.getMid());
			if (request.getMid() == 0) {
				lo.put("interf", request.getC());
				lo.put("method", request.getM() + "-" + request.getArgTypes().length);
			} else {
				Method m = ServerContext.server().methodCache().getMethod(request.getMid());
				lo.put("method", m.getName() + m.getParameterTypes().length);
			}
			if (GameBootstrap.printDetail) {
				lo.put("args", JSONObject.toJSONString(request.getArgs()));
			}
		}
		logger.info(lo.toJSONString());
	}

	private void logBeat(ChannelHandlerContext ctx) {
		JSONObject lo = Loggers.newLogObj(Loggers.PING_FROM);
		lo.put("from", ctx.channel().remoteAddress());
		logger.debug(lo.toJSONString());
	}

	private DecodeStat getDecodeStat(Channel channel) {
		DecodeStat decodeStat = channel.attr(decodeStatKey).get();
		if (decodeStat == null) {
			decodeStat = new DecodeStat();
			channel.attr(decodeStatKey).set(decodeStat);
		}
		return decodeStat;
	}
	
	private static AttributeKey<DecodeStat> decodeStatKey = AttributeKey.valueOf("decodeStat");
	
	private static class DecodeStat {
		volatile boolean OK = true;
		volatile long lastId;
	}
}
