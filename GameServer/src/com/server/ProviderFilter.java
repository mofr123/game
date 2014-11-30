package com.server;


public class ProviderFilter {

	public Object filter(FilterContext filterContext) throws Throwable {
		return filterContext.fireFliter();
	}

}