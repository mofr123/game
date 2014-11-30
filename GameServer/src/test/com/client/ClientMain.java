package test.com.client;

import java.io.IOException;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import test.com.interf.Hello;

public class ClientMain {

	public static void main(String[] args) throws IOException {
		ApplicationContext context = new ClassPathXmlApplicationContext(
				"test/com/client/hello-client.xml");
		Hello hello = (Hello) context.getBean("hello");
		System.out.println(hello.hello("baby"));
		hello.hello("");
		System.in.read();
	}

}