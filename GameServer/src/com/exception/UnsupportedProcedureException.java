package com.exception;

public class UnsupportedProcedureException extends GameRuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3949786060993602127L;

	public static UnsupportedProcedureException create(String interf, String method, String message) {
		String t = String.format("provider of interf:%s does not support method:%s,cause:%s", interf, method, message);
		UnsupportedProcedureException ume = new UnsupportedProcedureException(t);
		return ume;
	}

	public UnsupportedProcedureException(String message) {
		super(message);
	}
}
