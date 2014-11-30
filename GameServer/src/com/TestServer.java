package com;


import java.net.URL;
import java.util.Date;

import org.apache.log4j.PropertyConfigurator;

import com.data.bean.User;
import com.data.dao.UserDao;


public class TestServer {

	private UserDao dao = new UserDao();
	
	public static void main(String[] args) {
		TestServer t = new TestServer();
		t.load();
		
	}

	
	public void load(){
		//User u= dao.select("mo",1);
		User u = new User();
		u.setUserid(1012L);
		u.setUsername("game");
		u.setCreatetime(new Date());
		u.setLastlogintime(new Date());
		u.setAd("aa");
		u.setServer(2);
		u.setIsForbid(1);
		dao.insert(u);
		System.out.println(u.getUserid()+":"+u.getCreatetime());
	}
	
	public void test1(){
		URL resouceUrl = this.getClass().getResource(
				"/config/log4j.properties");
		PropertyConfigurator.configure(resouceUrl);
		//ServerListen s = new ServerListen();
		//s.start();
		//SendMessage t = new SendMessage();
		//t.run();
	}
}
