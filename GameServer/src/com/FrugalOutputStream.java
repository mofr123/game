package com;

import java.io.ByteArrayOutputStream;

public class FrugalOutputStream extends ByteArrayOutputStream {

	public FrugalOutputStream(int size) {
		super(size);
	}

	public byte[] buf() {
		return super.buf;
	}

//	public void writeAdler32(long value) {
//		write((int) (value >> 24));
//		write((int) (value >> 16));
//		write((int) (value >> 8));
//		write((int) value);
//	}

	public void flushSize() {
		int size = count - 5;
		buf[2] = (byte) (size >> 16);
		buf[3] = (byte) (size >> 8);
		buf[4] = (byte) size;
	}
}
