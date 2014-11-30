package com;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;

public class Requests {

	private static AtomicLong lastId = new AtomicLong(0);

	private static int rate = 100;

	public static long nextId() {
		for (;;) {
			long last = lastId.get();
			long now = System.currentTimeMillis() * rate;
			if (last >= now) {
				now = last + 1;
			}
			if (lastId.compareAndSet(last, now)) {
				return now;
			}
		}
	}

	public static boolean inRetryTime(Request request) {
		long ct = createTime(request);
		long now = System.currentTimeMillis();
		return ct > now - TimeUnit.SECONDS.toMillis(15);
	}
	
	public static boolean inHandleTime(Request request) {
		long ct = createTime(request);
		long now = System.currentTimeMillis();
		return ct > now - TimeUnit.SECONDS.toMillis(15) && ct < now + TimeUnit.SECONDS.toMillis(15);
	}

	public static long createTime(Request request) {
		return request.getId() / rate;
	}

	public static long createTime(Respone respone) {
		return respone.getId() / rate;
	}
}
