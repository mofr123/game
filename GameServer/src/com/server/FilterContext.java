package com.server;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Collections;
import java.util.List;

public class FilterContext {

	private Provider provider;

	private ProviderExceptionResolver exceptionResolver;

	private ClientInfo clientInfo;

	private Method targetMethod;

	private Object[] args;

	private List<ProviderFilter> filters = Collections.emptyList();

	private int index;

	public FilterContext(Provider provider, ClientInfo clientInfo, Method targetMethod, Object[] args,
			List<ProviderFilter> filters, ProviderExceptionResolver exceptionResolver) {
		this.provider = provider;
		this.clientInfo = clientInfo;
		this.targetMethod = targetMethod;
		this.args = args;
		this.filters = filters;
		this.exceptionResolver = exceptionResolver;
	}

	/**
	 * 执行下一个Fliter
	 * @return
	 * @throws Throwable
	 */
	public Object fireFliter() throws Throwable {
		if (index == filters.size()) {
			try {
				return targetMethod.invoke(provider.getTarget(), args);
			} catch (InvocationTargetException e) {
				if (exceptionResolver != null) {
					exceptionResolver.resolveException(provider, clientInfo, targetMethod, args, e.getTargetException());
				}
				throw e.getTargetException();
			}
		}
		return filters.get(index++).filter(this);
	}

	public Object getProvider() {
		return provider;
	}

	public ClientInfo getClientInfo() {
		return clientInfo;
	}

	public Method getTargetMethod() {
		return targetMethod;
	}

	public Object[] getArgs() {
		return args;
	}

	public List<ProviderFilter> getFilters() {
		return filters;
	}

	public int getIndex() {
		return index;
	}
}
