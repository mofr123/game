package com.server;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.Executors;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.core.Ordered;
import org.springframework.util.ClassUtils;

import com.ChannelId;
import com.Paths;
import com.Request;
import com.Requests;
import com.Respone;
import com.alibaba.fastjson.JSONObject;
import com.exception.TimeoutException;
import com.exception.ProvideTimeoutException;
import com.exception.ProviderOverloadException;
import com.exception.ProviderRegisterException;
import com.exception.UnsupportedProcedureException;
import com.google.common.base.Preconditions;
import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
//import com.monitor.GameMonitor;
//import com.spring.GameBootstrap;
import com.util.Loggers;

public class ServerContext {

	private static Logger logger = Logger.getLogger(ServerContext.class);

	private static ServerContext server;

	public static ServerContext server() {
		if (server == null) {
			synchronized (ServerContext.class) {
				if (server == null) {
					server = new ServerContext();
				}
			}
		}
		return server;
	}

	private ThreadPoolExecutor executor;
	private CompletionService<Respone> completionService;
	private ConcurrentMap<String, RequestPack> hanldingRequests;

	private ThreadPoolExecutor RunnerPool;

	private ServerBootstrap bootstrap;

	private ConcurrentMap<Class<?>, Provider> providers;
	private ScheduledExecutorService monitorExe;

	private ProviderExceptionResolver exceptionResolver;
	private List<ProviderFilter> filters = Lists.newArrayList();

	//private RegisterCenter registerCenter;
	//private GameMonitor gameMonitor;

	private MethodCache methodCache;

	private int port = 8099;

	private String _org;
	private String app;

	private int timeout;

	private String zk;
	private String zkroot;

	private AtomicBoolean start = new AtomicBoolean(false);
	private volatile boolean shutdown = false;

	private AtomicInteger in = new AtomicInteger(0);
	private AtomicInteger out = new AtomicInteger(0);

	public void start() throws Throwable {
		if (StringUtils.isBlank(_org) || StringUtils.isBlank(app) || StringUtils.isBlank(zk)) {
			logger.error("illegal server argument");
			throw new IllegalArgumentException("illegal server argument");
		}
		if (start.compareAndSet(false, true)) {//这里可以不用判断
			providers = Maps.newConcurrentMap();

			ServerBootstrap bootstrap = new ServerBootstrap();
			EventLoopGroup bossGroup = new NioEventLoopGroup();
			EventLoopGroup workerGroup = new NioEventLoopGroup();
			bootstrap.group(bossGroup, workerGroup).channel(NioServerSocketChannel.class);
			bootstrap.childHandler(new ServerChannelInitializer());
			bootstrap.option(ChannelOption.SO_BACKLOG, 128).childOption(ChannelOption.SO_KEEPALIVE, true);
			bootstrap.option(ChannelOption.TCP_NODELAY, true);
			ChannelFuture cf = bootstrap.bind(port);
			try {
				cf.await();
			} catch (InterruptedException e) {
				logger.error("server is interrputed");
				throw new RuntimeException("server is interrputed");
			}
			if (cf.cause() != null) {
				throw cf.cause();
			}
			this.bootstrap = bootstrap;

			JSONObject lo = Loggers.newLogObj(Loggers.NETTY_SERVER);
			String host = InetAddress.getLocalHost().getHostAddress();
			lo.put("hostPort", host + ":" + port);
			logger.info(lo.toJSONString());

			methodCache = new MethodCache();

			//ZkClient zkClient = ZkClientFactory.create(zk, zkroot, _org, app, GameBootstrap.startTime);
			//registerCenter = new RegisterCenter(zkClient, port);

			if (executor == null) {
				executor = new ThreadPoolExecutor(20, 100, 120, TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(1));
				completionService = new ExecutorCompletionService<Respone>(executor);
			}

			hanldingRequests = Maps.newConcurrentMap();
			RunnerPool = (ThreadPoolExecutor) Executors.newFixedThreadPool(2);
			RunnerPool.execute(new ResponeRunner());
			RunnerPool.execute(new TimeoutRunner());
		}
	}

	public void shutdown() {
		this.shutdown = true;
		if (start.get()) {
			executor.shutdownNow();
			bootstrap.group().shutdownGracefully();
			bootstrap.childGroup().shutdownGracefully();
			//registerCenter.shutdown();
			monitorExe.shutdownNow();
			RunnerPool.shutdownNow();
			logger.info("server context shutdown");
		}
	}

	public ServerBootstrap bootstrap() {
		return bootstrap;
	}

	public void registerProvider(Class<?> interf, int timeout, Object target) {
		if (!start.get()) {
			logger.error("server does not start");
			throw new IllegalArgumentException("server does not start");
		}
		Preconditions.checkNotNull(interf);
		Preconditions.checkNotNull(target);
		if (timeout < 0) {
			timeout = this.timeout;
		}
		Provider newProvider = new Provider(_org, app, timeout, interf, target);
		if (providers.put(interf, newProvider) != null) {
			String message = "provider of " + interf.getCanonicalName() + " has registered";
			logger.error(message);
			throw new ProviderRegisterException(message);
		}
		if (!interf.isInterface()) {
			String message = "provider " + target + "'interf:" + interf.getCanonicalName() + " is not a interface";
			logger.error(message);
			throw new ProviderRegisterException(message);
		}
		@SuppressWarnings("rawtypes")
		List<Class> interfs = Lists.newArrayList(ClassUtils.getAllInterfaces(target));
		if (!interfs.contains(interf)) {
			String message = "provider " + target + " does not support interf:" + interf.getCanonicalName();
			logger.error(message);
			throw new ProviderRegisterException(message);
		}
		methodCache.putMethods(interf);
		//registerCenter.register(newProvider);
	}

	/**
	 * 配置,启动,注册结束
	 */
	public void configureEnd() {
		logger.info("app:{}, timeout:{}"+Paths.joinPath(_org, app)+ timeout);
		logger.info("zk:{}, zkroot:{}"+zk+ zkroot);
		try {
			logger.info("server_address: {}:{}"+InetAddress.getLocalHost().getHostAddress()+ port);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}

		// providers
		StringBuilder sb = new StringBuilder();
		sb.append("providers:[");
		for (Provider provider : providers.values()) {
			sb.append(provider.getInterf().getCanonicalName());
			if (provider.getTimeout() > 0) {
				sb.append("(");
				if (provider.getTimeout() > 0) {
					sb.append(provider.getTimeout());
				}
				sb.append(")");
			}
			sb.append(",");
		}
		if (providers.size() > 0) {
			sb.deleteCharAt(sb.length() - 1);
		}
		sb.append("]");
		logger.info(sb.toString());

		// filters
		sb = new StringBuilder();
		sb.append("filters:[");
		for (ProviderFilter filter : filters) {
			sb.append(filter.getClass()).append(",");
		}
		if (filters.size() > 0) {
			sb.deleteCharAt(sb.length() - 1);
		}
		sb.append("]");
		logger.info(sb.toString());

		// exceptionResolver
		logger.info("exceptionResolver:" + (exceptionResolver == null ? null : exceptionResolver.getClass().getName()));

		monitorExe = new ScheduledThreadPoolExecutor(1);
		monitorExe.scheduleWithFixedDelay(new ServerLogRunner(), 2, 60, TimeUnit.MINUTES);
	}

	public Respone preRequest(Request request, ClientInfo clientInfo) {
		Respone respone = Respone.from(request);
		clientInfo.setOrg(request.getOrg());
		clientInfo.setApp(request.getApp());
		clientInfo.setProps(request.getProps());
		return respone;
	}

	public Respone submitRequest(Request request, ClientInfo clientInfo, Channel channel) {
		in.getAndIncrement();
		if (hanldingRequests.containsKey(requestPackId(request.getChannelId(), request.getId()))) {
			return null;
		}
		// 找出对应provider,method
		if (!Requests.inHandleTime(request)) {
			Respone respone = Respone.from(request);
			respone.setTh(new TimeoutException(String.format(
					"request over time,request time:%s,server time:%s(diff must less then 15s)",
					Requests.createTime(request), System.currentTimeMillis())));
			return respone;
		}

		Provider provider = null;
		Method method = null;

		try {
			if (request.getMid() != 0) {
				method = methodCache.getMethod(request.getMid());
				request.setC(method.getDeclaringClass().getCanonicalName());
				request.setM(method.getName());
			} else {
				Class<?>[] argTypes = new Class<?>[request.getArgTypes().length];
				for (int i = 0; i < request.getArgTypes().length; i++) {
					argTypes[i] = org.apache.commons.lang.ClassUtils.getClass(request.getArgTypes()[i]);
				}
				Integer mid = methodCache.getMethodId(request.getC(), request.getM(), argTypes);
				request.setMid(mid);
				method = methodCache.getMethod(mid);
			}
			if (method == null) {
				Respone respone = Respone.from(request);
				responeException(respone, request.getC(), request.getM(), "not exist");
				return respone;
			}
			provider = providers.get(method.getDeclaringClass());
			if (provider == null) {
				Respone respone = Respone.from(request);
				responeException(respone, request.getC(), request.getM(), "none provider of " + request.getC() + "."
						+ request.getM());
				return respone;
			}
		} catch (Exception e) {
			logger.warn("", e);
			Respone respone = Respone.from(request);
			responeException(respone, request.getC(), request.getM(), e.getMessage());
			return respone;
		}

		RequestPack pack = new RequestPack(request, channel);
		hanldingRequests.put(requestPackId(ChannelId.getLocalId(channel), request.getId()), pack);

		try {
			completionService.submit(new HandleCaller(provider, method, request, clientInfo));
		} catch (RejectedExecutionException e) {
			Respone respone = Respone.from(request);
			ProviderOverloadException poe = new ProviderOverloadException("poolSize:" + executor.getPoolSize());
			respone.setTh(poe);
			return respone;
		}

		return null;
	}

	private Respone handleRequest0(Provider provider, Method method, Request request, ClientInfo clientInfo) {
		Respone respone = Respone.from(request);
		try {
			FilterContext filterContext = new FilterContext(provider, clientInfo, method, request.getArgs(), filters,
					exceptionResolver);
			Object result = filterContext.fireFliter();
			respone.setResult(result);
		} catch (Throwable e) {
			respone.setTh(e);
		}
		return respone;
	}

	private void responeException(Respone respone, String c, String m, String message) {
		UnsupportedProcedureException ume = UnsupportedProcedureException.create(c, m, message);
		respone.setTh(ume);
	}

	public ServerContext zk(String zk) {
		this.zk = zk;
		return this;
	}

	public ServerContext zkroot(String zkroot) {
		this.zkroot = zkroot;
		return this;
	}

	public ServerContext port(int port) {
		this.port = port;
		return this;
	}

	public ServerContext org(String org) {
		this._org = org;
		return this;
	}

	public ServerContext app(String app) {
		this.app = app;
		return this;
	}

	public ServerContext timeout(int timeout) {
		this.timeout = timeout;
		return this;
	}

	public ThreadPoolExecutor executor() {
		return executor;
	}

	public int in() {
		return in.intValue();
	}

	public int out() {
		return out.intValue();
	}

	public ServerContext executor(ThreadPoolExecutor executor) {
		this.executor = executor;
		return this;
	}
	public MethodCache methodCache() {
		return methodCache;
	}

	public ConcurrentMap<String, RequestPack> hanldingRequests() {
		return hanldingRequests;
	}

	public ProviderExceptionResolver exceptionResolver() {
		return this.exceptionResolver;
	}

	public void exceptionResolver(ProviderExceptionResolver exceptionResolver) {
		this.exceptionResolver = exceptionResolver;
	}

	/*public GameMonitor gameMonitor() {
		return gameMonitor;
	}

	public void setGameMonitor(GameMonitor gameMonitor) {
		this.gameMonitor = gameMonitor;
	}*/

	private String requestPackId(int channelId, Long id) {
		return channelId + "_" + id.longValue();
	}

	public void addFilter(ProviderFilter filter) {
		filters.add(filter);
		Comparator<ProviderFilter> com = new Comparator<ProviderFilter>() {
			@Override
			public int compare(ProviderFilter o1, ProviderFilter o2) {
				if (o1 instanceof Ordered && o2 instanceof Ordered) {
					return ((Ordered) o1).getOrder() - ((Ordered) o2).getOrder();
				} else if (o1 instanceof Ordered) {
					return -1;
				} else if (o2 instanceof Ordered) {
					return 1;
				}
				return 0;
			}
		};
		Collections.sort(filters, com);
	}
	
	private class HandleCaller implements Callable<Respone> {

		private Provider provider;
		private Method method;
		private Request request;
		private ClientInfo clientInfo;

		public HandleCaller(Provider provider, Method method, Request request, ClientInfo clientInfo) {
			this.provider = provider;
			this.method = method;
			this.request = request;
			this.clientInfo = clientInfo;
		}

		public Respone call() throws Exception {
			return handleRequest0(provider, method, request, clientInfo);
		}

	}

	private class ResponeRunner implements Runnable {
		@Override
		public void run() {
			for (;;) {
				if (shutdown) {
					break;
				}
				try {
					Respone respone = completionService.take().get();
					RequestPack requestPack = hanldingRequests.remove(requestPackId(respone.getChannelId(),
							respone.getId()));
					if (requestPack != null) {
						out.incrementAndGet();
						//Request r = requestPack.request;
						// 记录监控中心的服务
						/*gameMonitor.in(respone.getOrg(), respone.getApp(), r.getC(),
								Paths.methodName(r.getM(), r.getArgs() == null ? 0 : r.getArgs().length),
								requestPack.from, respone.getTh() != null);*/
						requestPack.channel.writeAndFlush(respone);
					} else {
						JSONObject jo = Loggers.newLogObj(Loggers.HANDLE_LOST);
						jo.put("id", respone.getId());
						jo.put("now", System.currentTimeMillis());
						logger.warn(jo.toJSONString());
					}
				} catch (InterruptedException e) {
				} catch (Exception e) {
					// nevel run here
					e.printStackTrace();
					logger.error("ResponeRunner ExecutionException", e.getCause());
				}
			}
		}
	}

	private class TimeoutRunner implements Runnable {
		@Override
		public void run() {
			for (;;) {
				if (shutdown) {
					break;
				}
				for (RequestPack pack : hanldingRequests.values()) {
					try {
						Long id = pack.request.getId();
						long t1 = Requests.createTime(pack.request);
						long now = System.currentTimeMillis();
						if (t1 + 600000 < now) {
							if (hanldingRequests
									.remove(requestPackId(pack.request.getChannelId(), pack.request.getId())) != null) {
								Respone r = Respone.from(pack.request);
								r.setTh(new ProvideTimeoutException(String.format("{id : %s}", id)));

								JSONObject jo = Loggers.newLogObj(Loggers.HANDLE_TIMEOUT);
								jo.put("id", id);
								jo.put("m", pack.request.getC() + "." + pack.request.getM());
								logger.error(jo.toJSONString());

								//Request request = pack.request;
								//String mname = Paths.methodName(request.getM(),
										//request.getArgs() == null ? 0 : request.getArgs().length);
								//gameMonitor.in(r.getOrg(), r.getApp(), request.getC(), mname, pack.from, true);
								pack.channel.writeAndFlush(r);
							}
							continue;
						}
						if (t1 + 1000 < now) {
							Respone r = Respone.from(pack.request);
							r.setType(Respone.TYPE.RPC_HANDLING);
							pack.channel.writeAndFlush(r);
						}
					} catch (Exception e) {
						hanldingRequests.remove(requestPackId(pack.request.getChannelId(), pack.request.getId()));
						e.printStackTrace();
						logger.error("timeout respone exception", e);
					}
				}
				try {
					Thread.sleep(999);
				} catch (InterruptedException e) {
				}
			} // pack for
		} // ;;
	}

	private static class RequestPack {
		Request request;
		Channel channel;
		//long from = System.currentTimeMillis();

		public RequestPack(Request request, Channel channel) {
			this.request = request;
			this.channel = channel;
		}
	}
}
