package com.lock;

import java.util.concurrent.TimeoutException;

import com.exception.CancelException;


public class AsyncResult<T> {

	protected volatile T value;

	protected byte[] lock = new byte[] { 0 };

	public boolean isDone() {
		return lock[0] == 1;
	}

	public void reset() {
		this.value = null;
		lock[0] = 0;
	}

	public void set(T value) {
		this.value = value;
		synchronized (lock) {
			if (lock[0] == 0) {
				lock[0] = 1;
				lock.notifyAll();
			}
		}
	}

	public boolean isCancelled() {
		return lock[0] == -1;
	}

	public void cancel() {
		synchronized (lock) {
			lock[0] = -1;
			lock.notifyAll();
		}
	}

	public T get(long timeout) throws TimeoutException, CancelException {
		if (lock[0] == 1) {
			return value;
		}
		synchronized (lock) {
			if (lock[0] == 1) {
				return value;
			}
			try {
				if (timeout < 0) {
					lock.wait();
				} else {
					lock.wait(timeout);
					if (lock[0] == 0) {
						throw new TimeoutException();
					}
					if (lock[0] == -1) {
						throw new CancelException();
					}
				}
			} catch (InterruptedException e) {
				throw new CancelException();
			}
			return value;
		}
	}

	public int getState() {
		return lock[0];
	}

	public T getValue() {
		return value;
	}
}
