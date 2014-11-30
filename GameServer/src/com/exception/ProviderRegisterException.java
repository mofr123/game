package com.exception;

public class ProviderRegisterException extends GameRuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3436368430692285666L;

	public ProviderRegisterException(Throwable cause) {
		super(cause);
	}
	
	public ProviderRegisterException(String message) {
		super(message);
	}
}
