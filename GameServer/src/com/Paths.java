package com;

import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.UnknownHostException;

//import com.client.ProviderMeta;

public class Paths {

	public static String joinPath(String... paths) {
		StringBuilder sb = new StringBuilder();
		for (String path : paths) {
			if ((sb.length() == 0 || sb.charAt(sb.length() - 1) != '/') && !path.startsWith("/")) {
				sb.append("/");
			}
			sb.append(path);
		}
		return sb.toString();
	}

	public static String encodeApp(Long zid, long t) throws UnknownHostException {
		StringBuilder sb = new StringBuilder();
		sb.append("ip=").append(InetAddress.getLocalHost().getHostAddress());
		sb.append("&zid=").append(zid);
		sb.append("&t=").append(t);
		return sb.toString();
	}
	
	public static String encodeProvider(Long zid, int port, Class<?> interf) throws UnknownHostException {
		StringBuilder sb = new StringBuilder();
		sb.append("zid=").append(zid);
		sb.append("&host=").append(InetAddress.getLocalHost().getHostAddress());
		sb.append("&port=").append(port);
		sb.append("&methods=");
		for (Method method : interf.getMethods()) {
			sb.append(methodName(method)).append(",");
		}
		if (sb.charAt(sb.length() - 1) == ',') {
			sb.deleteCharAt(sb.length() - 1);
		}
		return sb.toString();
	}
/*
	public static ProviderNode decodeProvider(String node) {
		QueryStringDecoder de = new QueryStringDecoder("?" + node);
		ProviderNode n = new ProviderNode();
		n.setZid(Long.parseLong(de.parameters().get("zid").get(0)));
		n.setHost(de.parameters().get("host").get(0));
		n.setPort(Integer.parseInt(de.parameters().get("port").get(0)));
		Iterable<String> it = Splitter.on(",").trimResults().omitEmptyStrings()
				.split(de.parameters().get("methods").get(0));
		List<String> methods = Lists.newArrayList(it);
		n.setMethods(methods);
		return n;
	}
*/
	public static String encodeConsumer(String from, String name1, String name2, String to, int port, long zid, int cid) {
		StringBuilder sb = new StringBuilder();
		sb.append("zid=").append(zid);
		sb.append("&cid=").append(cid);
		sb.append("&from=").append(from);
		sb.append("&to=").append(to);
		sb.append("&port=").append(port);
		sb.append("&org=").append(name1);
		sb.append("&app=").append(name2);
		return sb.toString();
	}
	/*
	public static void creates(CuratorFramework client, String[] paths) throws Exception {
		int i = paths.length;
		// 获取哪级目录不存在
		for (; i > 0; i--) {
			if (client.checkExists().forPath(StringUtils.join(paths, "/", 0, i)) != null) {
				break;
			}
		}
		for (i++; i <= paths.length; i++) {
			try {
				client.create().forPath(StringUtils.join(paths, "/", 0, i));
			} catch (NodeExistsException e) {
			}
		}
	}
*/
	public static String hostPort(String host, int port) {
		return host + ":" + port;
	}

	public static String methodName(Method method) {
		return methodName(method.getName(), method.getParameterTypes().length);
	}

	public static String methodName(String m, int len) {
		return m + "-" + len;
	}
/*
	public static String interfPath(ProviderMeta providerMeta, String interf) {
		return joinPath(providerMeta.zkPath(), interf);
	}
	*/
}
