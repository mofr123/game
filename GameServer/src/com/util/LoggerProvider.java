package com.util;

import org.apache.log4j.Logger;

public class LoggerProvider{
	private static final Logger dbconsuminglog = Logger.getLogger("DBLOGCONSUMING");

	public static Logger getDblog() {
		return dbconsuminglog;
	}
}
