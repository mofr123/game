package com;

import com.caucho.hessian.io.Hessian2Input;
import com.caucho.hessian.io.Hessian2Output;


public class IoPuts {

	private static ThreadLocal<Hessian2Output> Hessian2OutputLocal = new ThreadLocal<Hessian2Output>() {
		@Override
		protected Hessian2Output initialValue() {
			return new Hessian2Output();
		}
	};

	private static ThreadLocal<Hessian2Input> Hessian2InputLocal = new ThreadLocal<Hessian2Input>() {
		@Override
		protected Hessian2Input initialValue() {
			return new Hessian2Input();
		}
	};

	private static ThreadLocal<FrugalOutputStream> FrugalOutputStreamLocal = new ThreadLocal<FrugalOutputStream>() {
		@Override
		protected FrugalOutputStream initialValue() {
			return new FrugalOutputStream(8192);
		}
	};

	public static Hessian2Output hessian2Output() {
		return Hessian2OutputLocal.get();
	}

	public static Hessian2Input hessian2Input() {
		return Hessian2InputLocal.get();
	}

	public static FrugalOutputStream frugalOutputStream() {
		return FrugalOutputStreamLocal.get();
	}
}
