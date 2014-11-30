package com.monitor;

import java.util.List;


public interface MonitorProvider {

	public void report(long id, String org, String app, List<InvokeStatistic> in, List<InvokeStatistic> out);

//	@Broadcast broadcast���ȶ�
	public void ping(long zid, String org, String app, List<OrgApp> depends, SysInfo sysInfo);
}