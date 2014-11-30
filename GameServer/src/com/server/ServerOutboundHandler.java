package com.server;

import org.apache.log4j.Logger;

import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerAdapter;
import io.netty.channel.ChannelHandlerContext;
//import io.netty.channel.ChannelOutboundHandlerAdapter;


@Sharable
public class ServerOutboundHandler extends ChannelHandlerAdapter {

	private static Logger logger = Logger.getLogger(ServerOutboundHandler.class);
	
	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
		logger.error("unexpected exception", cause);
	}
	
}
