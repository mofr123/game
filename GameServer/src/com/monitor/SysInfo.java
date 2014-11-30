package com.monitor;

import java.io.Serializable;

public class SysInfo implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1637157563521793409L;
	
	private String cpuRate = "-";
	private int processors;
	private long usedMem;
	private long allMem;
	private long time;
	
	public void setTime(long time) {
		this.time = time;
	}
	public long getTime() {
		return time;
	}
	public String getCpuRate() {
		return cpuRate;
	}
	public void setCpuRate(String cpuRate) {
		this.cpuRate = cpuRate;
	}
	public int getProcessors() {
		return processors;
	}
	public void setProcessors(int processors) {
		this.processors = processors;
	}
	public long getUsedMem() {
		return usedMem;
	}
	public void setUsedMem(long usedMem) {
		this.usedMem = usedMem;
	}
	public long getAllMem() {
		return allMem;
	}
	public void setAllMem(long allMem) {
		this.allMem = allMem;
	}
	
}
