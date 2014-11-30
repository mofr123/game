package com.monitor;

import java.util.concurrent.ConcurrentMap;

import com.google.common.collect.Maps;

public class AppStatistics {

	private long id;

	public AppStatistics(long id) {
		this.id = id;
	}

	private ConcurrentMap<InvokeStatistic, InvokeStatistic> inStatistics = Maps.newConcurrentMap();

	private ConcurrentMap<InvokeStatistic, InvokeStatistic> outStatistics = Maps.newConcurrentMap();

	public long getId() {
		return id;
	}

	public ConcurrentMap<InvokeStatistic, InvokeStatistic> getInStatistics() {
		return inStatistics;
	}

	public ConcurrentMap<InvokeStatistic, InvokeStatistic> getOutStatistics() {
		return outStatistics;
	}
}
