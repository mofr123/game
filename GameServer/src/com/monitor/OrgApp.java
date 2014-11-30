package com.monitor;

import java.io.Serializable;

public class OrgApp implements Comparable<OrgApp>, Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 8898243197262010407L;

	private String org;
	private String app;

	public OrgApp(String org, String app) {
		this.org = org;
		this.app = app;
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

	public boolean isMonitor() {
		return "monitor".equals(org) && "monitor".equals(app);
	}

	@Override
	public int hashCode() {
		final int prime = 7;
		int result = 1;
		result = prime * result + ((app == null) ? 0 : app.hashCode());
		result = prime * result + ((org == null) ? 0 : org.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		OrgApp other = (OrgApp) obj;
		if (app == null) {
			if (other.app != null)
				return false;
		} else if (!app.equals(other.app))
			return false;
		if (org == null) {
			if (other.org != null)
				return false;
		} else if (!org.equals(other.org))
			return false;
		return true;
	}

	@Override
	public int compareTo(OrgApp o) {
		int t = this.org.compareTo(o.org);
		if (t != 0) {
			return t;
		}
		return this.app.compareTo(o.app);
	}
}
