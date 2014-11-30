package com;

public class Respone {

	public static class TYPE {
		public static final String CLOSE = "-2";
		public static final String INVALID = "-1";
		public static final String OK = "1";
		public static final String PRE_RPC = "2";
		public static final String RPC = "3";
		public static final String RPC_HANDLING = "4";
		public static final String DECODE_FAIL = "5";
	}

	private static Respone _ok;
	private static Respone _close;
	private static Respone _invalid;

	private String org;
	private String app;
	
	private Long id;
	private int mid;
	private String Type;
	private Throwable th;
	private Object result;

	private int channelId;
	
	public static Respone from(Request request) {
		Respone respone = null;
		if (request.getType().equals(Request.Type.PRE_RPC)) {
			respone = Respone.prerpc();
		} else {
			respone = Respone.rpc();
		}
		respone.setMid(request.getMid());
		respone.setId(request.getId());
		respone.setChannelId(request.getChannelId());
		respone.setOrg(request.getOrg());
		respone.setApp(request.getApp());
		return respone;
	}
	
	public static Respone rpc() {
		Respone rpc = new Respone();
		rpc.Type = TYPE.RPC;
		return rpc;
	}

	public static Respone prerpc() {
		Respone pre_rpc = new Respone();
		pre_rpc.Type = TYPE.PRE_RPC;
		return pre_rpc;
	}

	public static Respone ok() {
		if (_ok == null) {
			_ok = new Respone();
			_ok.Type = TYPE.OK;
		}
		return _ok;
	}

	public static Respone close() {
		if (_close == null) {
			_close = new Respone();
			_close.Type = TYPE.CLOSE;
		}
		return _close;
	}

	public static Respone invalid() {
		if (_invalid == null) {
			_invalid = new Respone();
			_invalid.Type = TYPE.INVALID;
		}
		return _invalid;
	}

	public static Respone decodeFail(long lastId) {
		Respone r = new Respone();
		r.setId(lastId);
		r.setType(TYPE.DECODE_FAIL);
		return r;
	}
	
	public String getType() {
		return Type;
	}

	public void setType(String type) {
		Type = type;
	}

	public Object getResult() {
		return result;
	}

	public void setResult(Object result) {
		this.result = result;
	}

	public Throwable getTh() {
		return th;
	}

	public void setTh(Throwable th) {
		this.th = th;
	}

	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public int getMid() {
		return mid;
	}

	public void setMid(int mid) {
		this.mid = mid;
	}

	public int getChannelId() {
		return channelId;
	}

	public void setChannelId(int channelId) {
		this.channelId = channelId;
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
	
}
