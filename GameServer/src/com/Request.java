package com;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.lock.AsyncResult;

public class Request {

	public static class Type {
		public static final String PING = "1";
		public static final String PRE_RPC = "2";
		public static final String RPC = "3";
	}

	private static Request _ping;

	private String type;

	// channel props
	private String org;
	private String app;
	private Map<String, Object> props;

	// private int timeout;
	// rpc
	private Long id;
	private int mid = 0; //代替 class,method,argTypes
	
	private String c;
	private String m;
	private String[] argTypes;
	private Object[] args;

	// argTypes2
	private Class<?>[] argTypes2;

	// 执行时间长的请求,服务端会定时返回一个ping
	private long ping;
	private volatile int channelId;
	// 重试次数
	private int retry;

	// return
	private AsyncResult<Respone> asyncRespone;

	private List<String> channels = new ArrayList<String>(1);
	private List<Integer> channelIds = new ArrayList<Integer>(1);
	
	public static Request prerpc() {
		Request r = new Request();
		r.id = Requests.nextId();
		r.setType(Request.Type.PRE_RPC);
		return r;
	}

	public static Request ping() {
		if (_ping == null) {
			_ping = new Request();
			_ping.setType(Request.Type.PING);
		}
		return _ping;
	}

	public static Request rpc() {
		Request r = new Request();
		r.id = Requests.nextId();
		r.setType(Request.Type.RPC);
		return r;
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

	public Map<String, Object> getProps() {
		return props;
	}

	public void setProps(Map<String, Object> props) {
		this.props = props;
	}

	public int getMid() {
		return mid;
	}

	public void setMid(int mid) {
		this.mid = mid;
	}

	public String getC() {
		return c;
	}

	public void setC(String c) {
		this.c = c;
	}

	public String getM() {
		return m;
	}

	public void setM(String m) {
		this.m = m;
	}

	public Object[] getArgs() {
		return args;
	}

	public void setArgs(Object[] args) {
		this.args = args;
	}

	public String[] getArgTypes() {
		return argTypes;
	}

	public void setArgTypes(String[] argTypes) {
		this.argTypes = argTypes;
	}

	public Class<?>[] getArgTypes2() {
		return argTypes2;
	}

	public void setArgTypes2(Class<?>[] argTypes2) {
		this.argTypes2 = argTypes2;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public AsyncResult<Respone> getAsyncRespone() {
		return asyncRespone;
	}

	public void setAsyncRespone(AsyncResult<Respone> asyncRespone) {
		this.asyncRespone = asyncRespone;
	}

	public int getRetry() {
		return retry;
	}

	public void addRetry() {
		this.retry++;
	}

	public long getPing() {
		return ping;
	}

	public void refreshPing() {
		this.ping = System.currentTimeMillis();
	}

	public int getChannelId() {
		return channelId;
	}

	public void setChannelId(int channelId) {
		this.channelId = channelId;
	}

	public List<String> getChannels() {
		return channels;
	}

	public List<Integer> getChannelIds() {
		return channelIds;
	}
	
}
