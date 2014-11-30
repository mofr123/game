package com.monitor;

import java.io.Serializable;
import java.util.concurrent.atomic.AtomicInteger;

public class InvokeStatistic implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1127590411742978563L;

	private Long id;
	private String org;
	private String app;
	private String interf;
	private String m;

	private AtomicInteger num = new AtomicInteger();
	private AtomicInteger cost = new AtomicInteger();
	private AtomicInteger ex = new AtomicInteger();
	private int maxCost;
	
	public InvokeStatistic() {
	}

	public InvokeStatistic(String org, String app, String interf, String m) {
		this.org = org;
		this.app = app;
		this.interf = interf;
		this.m = m;
	}

	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public String getOrg() {
		return org;
	}

	public String getApp() {
		return app;
	}

	public String getInterf() {
		return interf;
	}

	public String getFullM() {
		if (interf == null || m == null) {
			return null;
		}
		return interf + "." + m;
	}

	public String getM() {
		return m;
	}

	public void addNum() {
		num.getAndIncrement();
	}

	public void addNum(int num) {
		this.num.addAndGet(num);
	}
	
	public void addCost(int cost) {
		this.cost.getAndAdd(cost);
	}

	public void addEx() {
		ex.getAndIncrement();
	}

	public void addEx(int ex) {
		this.ex.addAndGet(ex);
	}
	
	public int getNum() {
		return num.intValue();
	}

	public void setNum(int num) {
		this.num = new AtomicInteger(num);
	}

	public int getCost() {
		return cost.intValue();
	}

	public void setCost(int cost) {
		this.cost = new AtomicInteger(cost);
	}

	public int getEx() {
		return ex.intValue();
	}

	public void setEx(int ex) {
		this.ex = new AtomicInteger(ex);
	}

	public int getRt() {
		if (num.get() == 0) {
			return 0;
		}
		return cost.get() / num.get();
	}

	public int getAverageCost() {
		if (num.get() == 0) {
			return 0;
		}
		return cost.get() / num.get();
	}
	
	public void setOrg(String org) {
		this.org = org;
	}

	public void setApp(String app) {
		this.app = app;
	}

	public void setInterf(String interf) {
		this.interf = interf;
	}

	public void setM(String m) {
		this.m = m;
	}

	public int getMaxCost() {
		return maxCost;
	}

	public void setMaxCost(int maxCost) {
		if (maxCost > this.maxCost) {
			this.maxCost = maxCost;
		}
	}

	@Override
	public int hashCode() {
		final int prime = 3;
		int result = 1;
		result = prime * result + ((app == null) ? 0 : app.hashCode());
		result = prime * result + ((interf == null) ? 0 : interf.hashCode());
		result = prime * result + ((m == null) ? 0 : m.hashCode());
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
		InvokeStatistic other = (InvokeStatistic) obj;
		if (app == null) {
			if (other.app != null)
				return false;
		} else if (!app.equals(other.app))
			return false;
		if (interf == null) {
			if (other.interf != null)
				return false;
		} else if (!interf.equals(other.interf))
			return false;
		if (m == null) {
			if (other.m != null)
				return false;
		} else if (!m.equals(other.m))
			return false;
		if (org == null) {
			if (other.org != null)
				return false;
		} else if (!org.equals(other.org))
			return false;
		return true;
	}
}
