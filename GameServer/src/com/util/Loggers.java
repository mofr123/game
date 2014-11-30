package com.util;

import com.alibaba.fastjson.JSONObject;

public class Loggers {

	public static final String NETTY_SERVER = "netty_server";
	public static final String ZK_OK = "zk_ok";
	public static final String ZK_LOST = "zk_lost";

	public static final String REGISTER_P = "register_p";
	public static final String REGISTER_C = "register_c";
	public static final String REGISTER_C_ERR = "register_c_err";
	public static final String WATCHED = "watched";

	public static final String ACTIVE = "active";
	public static final String INACTIVE = "inactive";
	public static final String CONNECT = "connect";
	public static final String FREE = "free";
	public static final String LOST = "lost";
	public static final String PRE_REQUEST = "pre_request";

	public static final String INVOKE = "invoke";
	public static final String REQUEST_OUT = "request_out";
	public static final String RESPONE_IN = "respone_in";
	public static final String REQUEST_DISCARD = "request_discard";
	public static final String NONE_REQUEST = "none_request";
	public static final String REQUEST_IN = "request_in";
	public static final String RESPONE_OUT = "respone_out";
	
	public static final String HANDLE_TIMEOUT = "handle_timeout";
	public static final String HANDLE_LOST = "handle_lost";
	
	public static final String PING_FROM = "ping_from";
	public static final String PING_TO = "ping_to";

	public static final String M_ID = "m-id";

	public static final String ENCODE = "encode";
	public static final String DECODE = "decode";
	public static final String DECODE_ERR = "decode_err";

	public static final String RETRY = "retry";
	public static final String MAX_RETRY = "max_retry";
	
	public static final String EXECUTOR = "executor";

	public static JSONObject newLogObj(String type) {
		JSONObject obj = new JSONObject(5);
		obj.put("logType", type);
		return obj;
	}

	public static JSONObject newLogObj(String type, Throwable e) {
		JSONObject obj = new JSONObject(5);
		obj.put("logType", type);
		if (e != null) {
			obj.put("e", e.getClass().getName());
			obj.put("e-msg", e.getMessage());
		}
		return obj;
	}
}