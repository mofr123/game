package com.data;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.log4j.Logger;



/**
 * 数据库服务
 * @author Administrator
 *
 */
public class DataServer {
	private Logger log = Logger.getLogger(DataServer.class);
	
	private SqlSessionFactory sqlMapper;
	
	private static Object obj = new Object();
	
	private static DataServer server ;
	
	private DataServer(){
		try{
			String resource = "src/config/db-config.xml";
			InputStream in = new FileInputStream(resource);
			sqlMapper = new SqlSessionFactoryBuilder().build(in);
			in.close();
		}catch (IOException e) {
			log.error("DataServer()", e);
			e.printStackTrace();
		}
	}
	
	public static DataServer getInstance(){
		synchronized (obj) {
			if(server == null) server = new DataServer();
		}
		return server;
	}

	public SqlSessionFactory getSqlMapper() {
		return sqlMapper;
	}

	public void setSqlMapper(SqlSessionFactory sqlMapper) {
		this.sqlMapper = sqlMapper;
	}
	
	public static void main(String[] args) {
		new DataServer();
	}
}
