package com.server;

import com.Paths;


public class Provider {

	private String org;
	private String app;
	private int timeout;
	private Class<?> interf;
	private Object target;

	private volatile long lastRegister;

	public Provider(String org, String app, int timeout, Class<?> interf, Object target) {
		this.org = org;
		this.app = app;
		this.timeout = timeout;
		this.interf = interf;
		this.target = target;
	}

	public String getOrg() {
		return org;
	}

	public void setOrg(String org) {
		this.org = org;
	}

	public String getApp() {
		return app;
	}

	public void setApp(String app) {
		this.app = app;
	}

	public Class<?> getInterf() {
		return interf;
	}

	public void setInterf(Class<?> interf) {
		this.interf = interf;
	}

	public int getTimeout() {
		return timeout;
	}

	public void setTimeout(int timeout) {
		this.timeout = timeout;
	}

	public Object getTarget() {
		return target;
	}

	public void setTarget(Object target) {
		this.target = target;
	}

	@Override
	public int hashCode() {
		return Paths.joinPath(org, app, interf.getCanonicalName()).hashCode();
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null) {
			return false;
		}
		if (obj instanceof Provider) {
			Provider t = (Provider) obj;
			return this.org.equals(t.org) && this.app.equals(t.app) && this.interf == t.interf;
		}
		return false;
	}

	public long getLastRegister() {
		return lastRegister;
	}

	public void setLastRegister(long lastRegister) {
		this.lastRegister = lastRegister;
	}

}
