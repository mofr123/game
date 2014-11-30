package test.com.server;

import java.util.Random;
import java.util.concurrent.TimeUnit;

import org.apache.commons.lang.StringUtils;



import com.annot.Provider;

import test.com.interf.Hello;

@Provider
public class HelloImpl implements Hello {

	public String hello(String name) {
		
		try {
			TimeUnit.MILLISECONDS.sleep(new Random().nextInt(200));
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		if (StringUtils.isBlank(name)) {
			throw new RuntimeException("you are not welcome");
		}
		
		return "hello " + name;
	}

}
