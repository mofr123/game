package com.exception;

public class GameRuntimeException extends RuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4598960974878407909L;

	public GameRuntimeException() {
		super();
	}

	public GameRuntimeException(String message, Throwable cause) {
		super(message, cause);
	}

	public GameRuntimeException(String message) {
		super(message);
	}

	public GameRuntimeException(Throwable cause) {
		super(cause);
	}

	
}