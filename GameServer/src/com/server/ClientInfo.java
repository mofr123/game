package com.server;

import java.util.Collections;
import java.util.Map;

public class ClientInfo {

	private String org;
	private String app;
	private Map<String, Object> props = Collections.emptyMap();

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

	public Object getProp(String key) {
		return props.get(key);
	}
	
	public Map<String, Object> getProps() {
		return props;
	}
	public void setProps(Map<String, Object> props) {
		this.props = props;
	}
	
}
