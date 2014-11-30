package com;

import java.io.IOException;
import java.io.InputStream;
import java.util.zip.Adler32;

public class EncryptInputStream extends InputStream {

	private Adler32 adler32;
	private InputStream is;

	public EncryptInputStream(Adler32 adler32, InputStream is) {
		this.adler32 = adler32;
		this.is = is;
	}

	@Override
	public int read() throws IOException {
		int v = is.read();
		if (v != -1) {
			adler32.update(v);
		}
		return v;
	}

	@Override
	public int read(byte[] b) throws IOException {
		int num = is.read(b);
		if (num > 0) {
			adler32.update(b, 0, num);
		}
		return num;
	}

	@Override
	public int read(byte[] b, int off, int len) throws IOException {
		int num = is.read(b, off, len);
		if (num > 0) {
			adler32.update(b, off, num);
		}
		return num;
	}

	@Override
	public long skip(long n) throws IOException {
		return is.skip(n);
	}

	@Override
	public int available() throws IOException {
		return is.available();
	}

	@Override
	public void close() throws IOException {
		is.close();
	}

	@Override
	public synchronized void mark(int readlimit) {
		is.mark(readlimit);
	}

	@Override
	public synchronized void reset() throws IOException {
		is.reset();
	}

	@Override
	public boolean markSupported() {
		return is.markSupported();
	}

}
