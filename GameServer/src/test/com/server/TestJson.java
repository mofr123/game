package test.com.server;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;

public class TestJson {

	public static void main(String[] args) {
		String str = "{\"type\":\"type\",\"account\":\"123456x\",\"password\":\"123456x\"}";//:9dd3125a
		JSONObject json1 = JSON.parseObject(str);
		String str1 = (String) json1.get("type");
		String str2 = (String) json1.get("account");
		System.out.println(str1+"-"+str2);
	}

}
