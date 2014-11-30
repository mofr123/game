package com.spring;

import java.net.InetAddress;
import java.net.UnknownHostException;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanCreationException;
import org.springframework.beans.factory.CannotLoadBeanClassException;
import org.springframework.beans.factory.access.BootstrapException;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
//import org.springframework.beans.factory.config.RuntimeBeanReference;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.beans.factory.support.RootBeanDefinition;

import com.annot.Provider;
//import com.client.ClientContext;
import com.google.common.base.Preconditions;
//import com.monitor.GameMonitor;
//import com.monitor.MonitorProvider;
import com.server.ServerContext;

public class GameBootstrap implements BeanFactoryPostProcessor {

	private static Logger logger = Logger.getLogger(GameBootstrap.class);

	public static byte[] adler32key = "mofa,mofr".getBytes();
	//是否记录请求与结果的参数值
	public static boolean printDetail = false;

	public static long startTime;
	
	static {
		System.setProperty("org.hyperic.sigar.path", "E:\\all_workspace\\commor_jar\\other\\sigar");
	}
	
	public static String org;
	public static String app;
	public static String owner;
	
	public static String zk;
	public static String zkroot = "/com";

	public static int port = 8099;

	public static int timeout = 10000;

	//private String logbackXML = "candy-def.xml";

	// client,server,mix
	public static String model;

	private boolean isServer = false;

	public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
		if (StringUtils.isBlank(model)) {
			for (String beanName : beanFactory.getBeanDefinitionNames()) {
				BeanDefinition def = beanFactory.getBeanDefinition(beanName);
				if (SpringProvider.class.getCanonicalName().equals(def.getBeanClassName())) {
					isServer = true;
				} else {
					if (!StringUtils.isBlank(def.getBeanClassName())) {
						Class<?> beanClass = null;
						try {
							beanClass = Class.forName(def.getBeanClassName());
						} catch (ClassNotFoundException e) {
							CannotLoadBeanClassException ce = new CannotLoadBeanClassException(
									def.getResourceDescription(), beanName, def.getBeanClassName(), e);
							throw ce;
						}
						if (beanClass.getAnnotation(Provider.class) != null) {
							isServer = true;
						}
					}
				}
				if (isServer) {
					break;
				}
			}
		} else {
			if ("server".equalsIgnoreCase(model) || "mix".equalsIgnoreCase(model)) {
				isServer = true;
			}
		}

		try {
			InetAddress.getLocalHost();
		} catch (UnknownHostException e1) {
			throw new BeanCreationException(e1.getMessage());
		}

		 // 设置Bean定义 
		/*RootBeanDefinition monitorProvider = new RootBeanDefinition();
		monitorProvider.setBeanClassName(SpringConsumer.class.getCanonicalName());
		monitorProvider.getPropertyValues().addPropertyValue("org", "monitor");
		monitorProvider.getPropertyValues().addPropertyValue("app", "monitor");
		monitorProvider.getPropertyValues().addPropertyValue("interf", MonitorProvider.class.getCanonicalName());
		((DefaultListableBeanFactory) beanFactory).registerBeanDefinition(MonitorProvider.class.getCanonicalName(),
				monitorProvider);*/

		/*RootBeanDefinition gameMonitor = new RootBeanDefinition();
		gameMonitor.setBeanClassName(GameMonitor.class.getCanonicalName());
		gameMonitor.getConstructorArgumentValues().addIndexedArgumentValue(0, org);
		gameMonitor.getConstructorArgumentValues().addIndexedArgumentValue(1, app);
		gameMonitor.setInitMethodName("start");
		gameMonitor.getPropertyValues().addPropertyValue("monitorProvider",
				new RuntimeBeanReference(MonitorProvider.class.getCanonicalName()));
		((DefaultListableBeanFactory) beanFactory).registerBeanDefinition(GameMonitor.class.getCanonicalName(),
				gameMonitor);*/

		if (isServer) {//已经不要
			RootBeanDefinition serverProcessor = new RootBeanDefinition();
			serverProcessor.setBeanClassName(ProviderRegisterProcessor.class.getCanonicalName());
			((DefaultListableBeanFactory) beanFactory).registerBeanDefinition(
					ProviderRegisterProcessor.class.getCanonicalName(), serverProcessor);
		}

		/*RootBeanDefinition clientProcessor = new RootBeanDefinition();
		clientProcessor.setBeanClassName(ConsumerPreputProcessor.class.getCanonicalName());
		((DefaultListableBeanFactory) beanFactory).registerBeanDefinition(
				ConsumerPreputProcessor.class.getCanonicalName(), clientProcessor);*/
		try {
			startTime = System.currentTimeMillis();
			start();
		} catch (Throwable e) {
			BeansException be = new BootstrapException("start game failed", e);
			throw be;
		}

		//beanFactory.registerSingleton(ContainerListener.class.getCanonicalName(), new ContainerListener(isServer));
	}

	private void start() throws Throwable {
		try {
			Preconditions.checkArgument(StringUtils.isNotBlank(org) && !org.contains("/"), "invalid name1");
			Preconditions.checkArgument(StringUtils.isNotBlank(app) && !app.contains("/"), "invalid name2");
			Preconditions.checkArgument(StringUtils.isNotBlank(zk), "invalid zk");
		} catch (Exception e) {
			logger.error(e.getMessage());
			throw e;
		}

		if (zkroot == null) {
			zkroot = "";
		}
		if (!zkroot.startsWith("/")) {
			zkroot = "/" + zkroot;
		}

		// client
/*		ClientContext client = ClientContext.client();
		client.org(org).app(app).timeout(timeout);
		client.zk(zk).zkroot(zkroot);
		client.start();*/
		// server
		//if (isServer) {
			ServerContext server = ServerContext.server();
			server.port(port).org(org).app(app).timeout(timeout);
			server.zk(zk).zkroot(zkroot);

			try {
				server.start();
			} catch (Throwable e) {
				e.printStackTrace();
				throw e;
			}
		//}

	}

	public String getOrg() {
		return org;
	}

	public void setOrg(String org) {
		GameBootstrap.org = org;
	}

	public void setApp(String app) {
		GameBootstrap.app = app;
	}

	public void setOwner(String owner) {
		GameBootstrap.owner = owner;
	}

	public void setZk(String zk) {
		GameBootstrap.zk = zk;
	}

	public void setZkroot(String zkroot) {
		GameBootstrap.zkroot = zkroot;
	}

	public void setModel(String model) {
		if (!"client".equalsIgnoreCase(model) && !"server".equalsIgnoreCase(model) && !"mix".equalsIgnoreCase(model)) {
			String message = "illegal model:" + model + "(client,server,mix)";
			throw new IllegalArgumentException(message);
		}
		GameBootstrap.model = model;
	}

	public void setPort(int port) {
		GameBootstrap.port = port;
	}

	public void setTimeout(int timeout) {
		if (timeout < 1000) {
			timeout = 1000;
		}
		GameBootstrap.timeout = timeout;
	}
/*
	public String getLogbackXML() {
		return logbackXML;
	}

	public void setLogbackXML(String logbackXML) {
		this.logbackXML = logbackXML;
	}*/

	public void setAdler32key(String adler32key) {
		GameBootstrap.adler32key = adler32key.getBytes();
	}

	public boolean isPrintDetail() {
		return printDetail;
	}

	public void setPrintDetail(boolean printDetail) {
		GameBootstrap.printDetail = printDetail;
	}

}
