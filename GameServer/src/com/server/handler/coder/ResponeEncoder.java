package com.server.handler.coder;

import org.apache.log4j.Logger;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;




import com.FrugalOutputStream;
import com.IoPuts;
import com.Respone;
import com.alibaba.fastjson.JSONObject;
import com.caucho.hessian.io.Hessian2Output;
import com.spring.GameBootstrap;
import com.util.Loggers;

public class ResponeEncoder extends MessageToByteEncoder<Respone> {

	private static Logger logger = Logger.getLogger(ResponeEncoder.class);

	@Override
	protected void encode(ChannelHandlerContext ctx, Respone respone, ByteBuf out) throws Exception {

		if (Respone.TYPE.OK.equals(respone.getType())) {
			out.writeByte(Byte.MAX_VALUE);
			ctx.writeAndFlush(out);
			return;
		}

		try {
			Hessian2Output hessout = IoPuts.hessian2Output();
			FrugalOutputStream fos = IoPuts.frugalOutputStream();
			fos.reset();
			fos.write(new byte[] { 1, 0, 0, 0, 0 });
			hessout.init(fos);
			hessout.startMessage();

			if (logger.isInfoEnabled()) {
				logResp(ctx, respone);
			}

			if (Respone.TYPE.PRE_RPC.equals(respone.getType())) {
				// pre_rpc
				hessout.writeString(Respone.TYPE.PRE_RPC);
				hessout.writeLong(respone.getId());
			} else if (Respone.TYPE.RPC.equals(respone.getType())) {
				// rpc
				hessout.writeString(Respone.TYPE.RPC);
				hessout.writeLong(respone.getId());
				hessout.writeInt(respone.getMid());
				hessout.writeObject(respone.getResult());
				if (respone.getTh() != null) {
					respone.getTh().setStackTrace(new StackTraceElement[0]);
				}
				hessout.writeObject(respone.getTh());
			} else if (Respone.TYPE.RPC_HANDLING.equals(respone.getType())) {
				// rpc_handling
				hessout.writeString(Respone.TYPE.RPC_HANDLING);
				hessout.writeLong(respone.getId());
			} else if (Respone.TYPE.DECODE_FAIL.equals(respone.getType())) {
				// decode fail
				hessout.writeString(Respone.TYPE.DECODE_FAIL);
				hessout.writeLong(respone.getId());
			}
			hessout.completeMessage();
			hessout.flush();
			fos.flushSize();

			if (logger.isDebugEnabled()) {
				logSize(respone, fos.size());
			}

			ByteBuf buf = ctx.alloc().buffer(fos.size());
			buf.writeBytes(fos.buf(), 0, fos.size());
			ctx.writeAndFlush(buf);

		} catch (Exception e) {
			JSONObject lo = Loggers.newLogObj(Loggers.ENCODE, e);
			logger.error(lo.toJSONString(), e);
		}
	}

	private void logResp(ChannelHandlerContext ctx, Respone respone) {
		JSONObject lo = Loggers.newLogObj(Loggers.RESPONE_OUT);
		lo.put("id", respone.getId());
		lo.put("to", ctx.channel().remoteAddress());
		lo.put("type", respone.getType());
		if (respone.getTh() != null) {
			lo.put("e", respone.getTh().getMessage());
		}
		if (GameBootstrap.printDetail) {
			lo.put("result", JSONObject.toJSONString(respone.getResult()));
		}
		logger.info(lo.toJSONString());
	}

	private void logSize(Respone respone, int size) {
		JSONObject lo = Loggers.newLogObj(Loggers.DECODE);
		lo.put("id", respone.getId());
		lo.put("size", size);
		logger.debug(lo.toJSONString());
	}

}
