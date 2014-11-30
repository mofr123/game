package com.exception;

public class CancelException extends GameException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 2955457385731329059L;

	public CancelException() {
		
	}
	
	public CancelException(String message) {
		super(message);
	}
}
