package com;

public class MethodMeta {

	private String interf;

	private String methodName;

	private Class<?>[] argTypes;

	public MethodMeta(String interf, String methodName, Class<?>[] argTypes) {
		this.interf = interf;
		this.methodName = methodName;
		this.argTypes = argTypes;
	}

	public String getInterf() {
		return interf;
	}

	public String getMethodName() {
		return methodName;
	}

	public Class<?>[] getArgTypes() {
		return argTypes;
	}

	@Override
	public int hashCode() {
		return (interf + "." + methodName).hashCode();
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj instanceof MethodMeta) {
			MethodMeta t = (MethodMeta) obj;
			if (!this.interf.equals(t.interf)) {
				return false;
			}
			if (!this.methodName.equals(t.methodName)) {
				return false;
			}
			if (this.argTypes == t.argTypes) {
				return true;
			}
			if (this.argTypes == null || t.argTypes == null) {
				return false;
			}
			if (this.argTypes.length != t.argTypes.length) {
				return false;
			}
			for (int i = 0; i < this.argTypes.length; i++) {
				if (this.argTypes[i] != t.argTypes[i]) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

}