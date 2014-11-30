package com.server;


import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpRequestDecoder;
import io.netty.handler.codec.http.HttpResponseEncoder;

import com.server.handler.ServerHandler;

public class ServerChannelInitializer extends ChannelInitializer<SocketChannel> {

	@Override
	protected void initChannel(SocketChannel ch) throws Exception {
		ChannelPipeline cp = ch.pipeline();
		cp.addLast("decoder", new HttpRequestDecoder());//new RequestDecoder()
		cp.addLast("encoder", new HttpResponseEncoder());//ResponeEncoder
		//cp.addLast("inboundHandler", new ServerInboundHandler());delete to 2014-11-28 16:31:22
		//cp.addLast("outboundHandler", new ServerOutboundHandler());
		cp.addLast("handler", new ServerHandler());
	}

}