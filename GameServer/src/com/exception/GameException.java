package com.exception;

public class GameException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1971128302061992080L;

	public GameException() {
		super();
	}

	public GameException(String message, Throwable cause) {
		super(message, cause);
	}

	public GameException(String message) {
		super(message);
	}

	public GameException(Throwable cause) {
		super(cause);
	}
	
}
