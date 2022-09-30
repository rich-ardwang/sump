package sumscope.lava.mq;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.ArrayList;
import java.util.Iterator;

public class AsyncThreadGroup<T> {
    private static int maxQueueSize = 20 * 10000;
    private Logger logger = LoggerFactory.getLogger(this.getClass());
    protected int threadCount;
    protected int curIndex = 0;
    protected ArrayList<AsyncBase<T>> list = new ArrayList();

    public AsyncThreadGroup(int threadCount) {
        this.threadCount = threadCount;

        for(int i = 0; i < threadCount; ++i) {
            AsyncBase<T> asyncBase = new AsyncBase<T>() {
                protected void process(T data) {
                    AsyncThreadGroup.this.process(data);
                }
            };
            this.list.add(asyncBase);
        }

    }

    protected synchronized int getIndex() {
        ++this.curIndex;
        if (this.curIndex < 0) {
            this.curIndex = 0;
        }
        return this.curIndex;
    }

    protected int findMostIdle() {
        int minCount = 2147483647;
        int index = -1;

        for(int i = 0; i < this.list.size(); ++i) {
            int count = ((AsyncBase)this.list.get(i)).getQueue().size();
            if (count <= 0) {
                index = i;
                break;
            }

            if (count < minCount) {
                minCount = count;
                index = i;
            }
        }

        return index;
    }

    public void add(T t) {
        this.add(t, this.findMostIdle());
    }

    public void add(T t, int index) {
        if (index < 0) {
            index = 0;
        }
        AsyncBase obj = (AsyncBase)this.list.get(index % this.threadCount);
        int nSize = obj.getQueue().size();
        if (nSize > maxQueueSize) {
            logger.error("The queue size of AsyncThreadGroup exceed {}! There must be blocked callback somewhere!", maxQueueSize);
        }
        obj.add(t);
    }

    protected void process(T t) {
    }

    public void dispose() {
        Iterator iter = this.list.iterator();
        while (iter.hasNext()) {
            AsyncBase<T> item = (AsyncBase)iter.next();
            item.dispose();
        }
    }

    public int size() {
        return this.threadCount;
    }
}
