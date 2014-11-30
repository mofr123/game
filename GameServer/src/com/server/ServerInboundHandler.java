package com.server;

import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.util.AttributeKey;

import java.io.IOException;




import org.apache.log4j.Logger;

import com.ChannelId;
import com.Request;
import com.Respone;
import com.alibaba.fastjson.JSONObject;
import com.util.Loggers;

@Sharable
public class ServerInboundHandler extends SimpleChannelInboundHandler<Request> {

	private static Logger logger = Logger.getLogger(ServerInboundHandler.class);

	@Override
	public void channelActive(ChannelHandlerContext ctx) throws Exception {
		ChannelId.getLocalId(ctx.channel());
		if (logger.isInfoEnabled()) {
			JSONObject lo = Loggers.newLogObj(Loggers.ACTIVE);
			lo.put("from", ctx.channel().remoteAddress());
			logger.info(lo.toJSONString());
		}
	}

	@Override
	public void channelInactive(ChannelHandlerContext ctx) throws Exception {
		if (logger.isInfoEnabled()) {
			JSONObject lo = Loggers.newLogObj(Loggers.INACTIVE);
			lo.put("from", ctx.channel().remoteAddress());
			logger.info(lo.toJSONString());
		}
	}

	public void channelRead0(ChannelHandlerContext ctx, Request msg) throws Exception {
		msg.setChannelId(ChannelId.getLocalId(ctx.channel()));
		Respone respone = null;
		if (Request.Type.PING.equals(msg.getType())) {
			respone = Respone.ok();
		} else if (Request.Type.PRE_RPC.equals(msg.getType())) {
			ClientInfo clientInfo = clientInfo(ctx.channel());
			respone = ServerContext.server().preRequest(msg, clientInfo);
		} else {
			ClientInfo clientInfo = clientInfo(ctx.channel());
			msg.setOrg(clientInfo.getOrg());
			msg.setApp(clientInfo.getApp());
			respone = ServerContext.server().submitRequest(msg, clientInfo, ctx.channel());
			if (respone != null) {
				logger.warn("submit request exception:", respone.getTh());
			}
		}
		if (respone != null) {
			ctx.writeAndFlush(respone);
		}
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
		if (cause instanceof IOException && cause.getMessage().contains("远程主机强迫关闭了一个现有的连接")) {
			// close quietly
		} else {
			logger.error("error", cause);
		}
	}

	static AttributeKey<ClientInfo> clientCachedKey = AttributeKey.valueOf("ClientInfo");
	
	static ClientInfo clientInfo(Channel channel) {
		ClientInfo cc = channel.attr(clientCachedKey).get();
		if (cc == null) {
			cc = new ClientInfo();
			channel.attr(clientCachedKey).set(cc);
		}
		return cc;
	}

	@Override
	protected void messageReceived(ChannelHandlerContext arg0, Request arg1)
			throws Exception {
		// TODO Auto-generated method stub
		
	}
}