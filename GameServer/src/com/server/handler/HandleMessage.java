package com.server.handler;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.data.bean.User;
import com.data.dao.UserDao;

public class HandleMessage {
	private static String message = null;
	private static Object m;
	private Logger logger = Logger.getLogger(HandleMessage.class);

	public HandleMessage(String message, Object m) {
		this.message = message;
		this.m = m;
		init();
	}
	
	public void init(){
		//"{\"type\":\"type\",\"account\":\"123456x\",\"password\":\"123456x\"}";//:9dd3125a
		JSONObject json1 = JSON.parseObject(m.toString());
		String type = (String) json1.get("type");
		String account = (String) json1.get("account");
		String pwd = (String) json1.get("account");
		UserDao dao = new UserDao();
		User u = dao.select(account, 1);
		if(u!=null){
			System.out.println("ok.............");
		}
	}
}
