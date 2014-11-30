package com.data.dao;

import java.util.HashMap;

import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.log4j.Logger;

import com.data.BaseDao;
import com.data.bean.User;
import com.util.LoggerProvider;
import com.util.TimeUtil;

public class UserDao extends BaseDao {
	
	private Logger log = Logger.getLogger(UserDao.class);

	private SqlSessionFactory sqlMapper = getSqlMapper();
	
    public int insert(User user) {
        SqlSession session = sqlMapper.openSession();
    	try{
    		long currentTimeMillis = System.currentTimeMillis();
    		long start = System.currentTimeMillis();
    		int rows = session.insert("game_user.insert", user);
    		session.commit();
    		long end = System.currentTimeMillis();
        	log.error("insert user cost:" + (end - start));
        	LoggerProvider.getDblog().info("game_user.insert "+TimeUtil.getDurationToNow(currentTimeMillis)+"ms");
    		return rows;
    	}finally{
			session.close();
		}
    }
    
    public User select(String username, int server) {
    	SqlSession session = sqlMapper.openSession();
    	try{
    		long currentTimeMillis = System.currentTimeMillis();
    		long start = System.currentTimeMillis();
    		HashMap<String, Object> map = new HashMap<String, Object>();
    		map.put("userName", username);
    		map.put("server", server);
    		User user = (User)session.selectOne("game_user.select", map);
    		long end = System.currentTimeMillis();
        	log.error("select user cost:" + (end - start));
        	LoggerProvider.getDblog().info("game_user.select "+TimeUtil.getDurationToNow(currentTimeMillis)+"ms");
            return user;
    	}finally{
			session.close();
		}
    }

    public int update(User user) {
        SqlSession session = sqlMapper.openSession();
    	try{
    		long currentTimeMillis = System.currentTimeMillis();
    		int rows = session.update("game_user.update", user);
    		session.commit();
    		LoggerProvider.getDblog().info("game_user.update "+TimeUtil.getDurationToNow(currentTimeMillis)+"ms");
            return rows;
    	}finally{
			session.close();
		}
    }
}