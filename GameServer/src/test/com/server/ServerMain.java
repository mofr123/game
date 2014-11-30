package test.com.server;

import java.io.IOException;

import org.springframework.context.support.ClassPathXmlApplicationContext;

public class ServerMain {

	public static void main(String[] args) throws IOException {
		new ClassPathXmlApplicationContext("test/com/server/hello-server.xml");
		//System.in.read();
	}
}
