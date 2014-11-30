package com.exception;

public class InvalidRegisterCenterException extends GameRuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 2665032409218643159L;
	
	public InvalidRegisterCenterException(String connectStr) {
		super("invalid register center:" + connectStr);
	}

}
