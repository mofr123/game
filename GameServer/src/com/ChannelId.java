package com;

import io.netty.channel.Channel;
import io.netty.util.AttributeKey;

import java.util.concurrent.atomic.AtomicInteger;

public class ChannelId {

	private static AtomicInteger NEXT_CHANNEL_ID = new AtomicInteger(1);
	private static AttributeKey<ChannelId> CHANNEL_ID_KEY = AttributeKey.valueOf("CHANNEL_ID_KEY");

	private int localId = NEXT_CHANNEL_ID.getAndIncrement();
	private long zkId;

	public static int getLocalId(Channel channel) {
		return getChannelId(channel).localId;
	}

	public static long getZkId(Channel channel) {
		return getChannelId(channel).zkId;
	}

	private static ChannelId getChannelId(Channel channel) {
		ChannelId id = channel.attr(CHANNEL_ID_KEY).get();
		if (id == null) {
			id = new ChannelId();
			channel.attr(CHANNEL_ID_KEY).set(id);
		}
		return id;
	}

	public int getLocalId() {
		return localId;
	}

	public long getZkId() {
		return zkId;
	}

	public void setZkId(long zkId) {
		this.zkId = zkId;
	}
	
}
