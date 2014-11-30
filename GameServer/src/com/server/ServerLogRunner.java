package com.server;


import org.apache.log4j.Logger;

import com.alibaba.fastjson.JSONObject;
import com.util.Loggers;

public class ServerLogRunner implements Runnable {

	private static Logger logger = Logger.getLogger(ServerLogRunner.class);

	public void run() {
		ServerContext server = ServerContext.server();

		int curSize = server.executor().getPoolSize();
		int maxSize = server.executor().getMaximumPoolSize();

		if (logger.isInfoEnabled() || curSize > maxSize * 0.6) {
			JSONObject lo = Loggers.newLogObj(Loggers.EXECUTOR);
			lo.put("threads", curSize);
			lo.put("threads_max", maxSize);
			lo.put("threads_largest", server.executor().getLargestPoolSize());
			lo.put("queue", server.executor().getQueue().size());
			lo.put("handling", server.hanldingRequests().size());
			lo.put("in", server.in());
			lo.put("out", server.out());
			if (curSize <= maxSize * 0.6) {
				logger.info(lo.toJSONString());
			} else if (curSize <= maxSize * 0.9) {
				logger.warn(lo.toJSONString());
			} else {
				logger.error(lo.toJSONString());
			}
		}

	}
}
