package com.exception;

public class DeserializeException extends GameRuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 4219201509107502928L;

	public DeserializeException(Exception e) {
		super(e);
	}

	public DeserializeException(String message) {
		super(message);
	}
}
