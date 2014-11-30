package com.exception;

public class InvalidWatcherCenterException extends GameRuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = -410981722908529024L;

	public InvalidWatcherCenterException(String connectStr) {
		super("invalid watcher center:" + connectStr);
	}
}
