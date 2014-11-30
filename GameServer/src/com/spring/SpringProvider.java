package com.spring;


public class SpringProvider {

	private Class<?> interf;
	private Object target;
	private int timeout = -1;
	
	public Class<?> getInterf() {
		return interf;
	}

	public void setInterf(Class<?> interf) {
		this.interf = interf;
	}

	public Object getTarget() {
		return target;
	}

	public void setTarget(Object target) {
		this.target = target;
	}

	public int getTimeout() {
		return timeout;
	}

	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}
}
