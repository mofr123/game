package com.server;

import java.lang.reflect.Method;

public interface ProviderExceptionResolver {

	/**
	 * you can throw your exception here,or it will throw initial exception
	 * 
	 * @param th
	 * @throws Throwable
	 */
	public void resolveException(Provider provider, ClientInfo clientCached, Method targetMethod, Object[] args,
			Throwable th) throws Throwable;

}