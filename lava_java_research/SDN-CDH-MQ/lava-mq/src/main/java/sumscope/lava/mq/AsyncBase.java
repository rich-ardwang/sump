package sumscope.lava.mq;

import java.util.LinkedList;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class AsyncBase<T> {
    protected final Logger logger;
    protected LinkedList<T> queue;
    protected Thread thread;
    protected boolean isStop;
    public String className;

    public AsyncBase() {
        this(true);
    }

    public AsyncBase(boolean startAtOnce) {
        this.logger = LoggerFactory.getLogger(this.getClass());
        this.isStop = false;
        this.className = null;
        this.queue = new LinkedList();
        this.thread = new Thread(new Runnable() {
            public void run() {
                while(!AsyncBase.this.isStop) {
                    try {
                        T data = null;
                        synchronized(AsyncBase.this.queue) {
                            if (AsyncBase.this.queue.size() <= 0) {
                                AsyncBase.this.queue.wait();
                            }

                            data = AsyncBase.this.queue.poll();
                        }

                        if (data != null) {
                            //String key = AsyncBase.this.toString();
                            AsyncBase.this.process(data);
                        }
                    } catch (Exception var5) {
                        AsyncBase.this.logger.warn("Deliver process error! e:{}.", var5);
                    }
                }

            }
        });
        if (startAtOnce) {
            this.start();
        } else {
            this.isStop = true;
        }
    }

    public Thread getThread() {
        return this.thread;
    }

    public LinkedList<T> getQueue() {
        return this.queue;
    }

    public void start() {
        this.logger.info("{} start.", this.className != null ? this.className : this.getClass());
        this.isStop = false;
        if (!this.thread.isAlive()) {
            this.thread.start();
        }
    }

    protected void process(T data) {
    }

    public void add(T t) {
        synchronized(this.queue) {
            this.queue.add(t);
            this.queue.notify();
        }
    }

    public void dispose() {
        this.isStop = true;
    }
}
