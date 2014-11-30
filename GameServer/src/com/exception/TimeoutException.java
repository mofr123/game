package com.exception;

public class TimeoutException extends GameRuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1090106736857581288L;

	public TimeoutException(String message) {
		super(message);
	}
}
