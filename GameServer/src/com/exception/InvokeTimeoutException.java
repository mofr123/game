package com.exception;

import com.Request;
import com.alibaba.fastjson.JSONObject;
import com.spring.GameBootstrap;

public class InvokeTimeoutException extends GameRuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 867464760336389292L;

	public static InvokeTimeoutException newEx(Request request) {
		String argsMessage = "";
		if (GameBootstrap.printDetail) {
			argsMessage = JSONObject.toJSONString(request.getArgs());
		}
		String message = String.format(
				"invoke timeout,method:%s,args:%s,id:%s,now:%s,ip:%s",
				new Object[] { request.getC() + "." + request.getM(), argsMessage, request.getId(),
						System.currentTimeMillis(), JSONObject.toJSONString(request.getChannels()) });
		InvokeTimeoutException e = new InvokeTimeoutException(message);
		return e;
	}

	public InvokeTimeoutException(String message) {
		super(message);
	}
}
