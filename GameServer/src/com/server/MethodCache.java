package com.server;

import java.lang.reflect.Method;
import java.util.Map;
import java.util.Set;







import org.apache.log4j.Logger;

import com.MethodMeta;
import com.alibaba.fastjson.JSONObject;
import com.exception.DuplicatedMethodException;
import com.google.common.collect.Maps;
import com.google.common.collect.Sets;
import com.util.Loggers;

/**
 * @author huangj
 *         用id代替class,argTypes,可以减少每次rpc的传递的数据量,同时可以减少Class.forName操作(当然,效果不是很明显
 *         )
 */
public class MethodCache {

	private static Logger logger = Logger.getLogger(MethodCache.class);

	private int nextId = 1;

	private Map<Integer, Method> idMap = Maps.newHashMap();

	private Map<MethodMeta, Integer> metaMap = Maps.newHashMap();

	public synchronized void putMethods(Class<?> interf) {
		Method[] ms = interf.getMethods();
		Set<String> exsits = Sets.newHashSet();
		for (Method method : ms) {
			String ns = method.getName() + method.getParameterTypes().length;
			if (exsits.contains(ns)) {
				String message = "sorry,candy does not support overload,method:" + ns;
				DuplicatedMethodException dme = new DuplicatedMethodException(message);
				throw dme;
			}
			exsits.add(ns);
		}
		for (Method method : ms) {
			MethodMeta meta = new MethodMeta(interf.getCanonicalName(), method.getName(), method.getParameterTypes());
			Integer id = nextId++;
			idMap.put(id, method);
			metaMap.put(meta, id);
			if (logger.isDebugEnabled()) {
				JSONObject lo = Loggers.newLogObj(Loggers.M_ID);
				lo.put("m", interf.getCanonicalName() + "." + method.getName() + method.getParameterTypes().length);
				lo.put("id", id);
				logger.debug(lo.toJSONString());
			}
		}
	}

	public synchronized Integer getMethodId(String interf, String name, Class<?>[] argTypes) {
		MethodMeta meta = new MethodMeta(interf, name, argTypes);
		return metaMap.get(meta);
	}

	public synchronized Method getMethod(Integer id) {
		return idMap.get(id);
	}

}
