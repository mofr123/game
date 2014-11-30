package com.util;

import java.net.URL;

import org.apache.log4j.PropertyConfigurator;

public class Utils {
	public static void configureLog4j(){
		URL resouceUrl = Utils.class.getClass().getResource(
				"/config/log4j.properties");
		PropertyConfigurator.configure(resouceUrl);
	}
	
	public static void main(String[] args) {
		Utils.configureLog4j();
	}
}
