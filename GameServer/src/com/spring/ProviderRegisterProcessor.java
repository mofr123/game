package com.spring;

import org.apache.log4j.Logger;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

import com.annot.Provider;
import com.exception.ProviderRegisterException;
//import com.monitor.GameMonitor;
import com.server.ProviderExceptionResolver;
import com.server.ProviderFilter;
import com.server.ServerContext;



public class ProviderRegisterProcessor implements BeanPostProcessor, ApplicationContextAware {

	private static Logger logger = Logger.getLogger(ProviderRegisterProcessor.class);

	private ApplicationContext applicationContext;

	public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
		return bean;
	}

	public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
		// @Provider 发布
		Provider provider = bean.getClass().getAnnotation(Provider.class);
		if (provider != null) {//下面已经不要 delete to 2014-11-28 15:23:41
			//lazyInitCandyMonitor();
			ServerContext server = ServerContext.server();
			Class<?>[] regInterfs = provider.interf();
			if (regInterfs != null && regInterfs.length > 0) {
				for (Class<?> interf : regInterfs) {
					server.registerProvider(interf, provider.timeout(), bean);
					return bean;
				}
			} else {
				regInterfs = bean.getClass().getInterfaces();
				if (regInterfs.length == 1) {
					server.registerProvider(regInterfs[0], provider.timeout(), bean);
				} else {
					logger.error("provider " + beanName + "'s interface can not be resolved");
					throw new ProviderRegisterException(String.format("provider %s's interface can not be resolved",
							beanName));
				}
			}
		} else {
			// SpringProvider发布
			if (bean instanceof SpringProvider) {
				//lazyInitCandyMonitor();
				SpringProvider p = (SpringProvider) bean;
				ServerContext.server().registerProvider(p.getInterf(), p.getTimeout(), p.getTarget());
			}
		}

		// exceptionResolver
		if (bean instanceof ProviderExceptionResolver) {
			if (ServerContext.server().exceptionResolver() != null) {
				throw new ProviderRegisterException("duplicated ProvideExceptionResolver");
			}
			ServerContext.server().exceptionResolver((ProviderExceptionResolver) bean);
		}
		
		if (bean instanceof ProviderFilter) {
			ServerContext.server().addFilter((ProviderFilter) bean);
		}

		// candyMonitor
		/*if (bean instanceof GameMonitor) {
			ServerContext.server().setGameMonitor((GameMonitor) bean);
		}*/
		return bean;
	}

	@Override
	public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
		this.applicationContext = applicationContext;
	}

	// CandyMonitor must be inited first?
	/*private void lazyInitCandyMonitor() {
		if (ServerContext.server().gameMonitor() == null) {
			applicationContext.getBeansOfType(GameMonitor.class);
		}
	}*/
}
