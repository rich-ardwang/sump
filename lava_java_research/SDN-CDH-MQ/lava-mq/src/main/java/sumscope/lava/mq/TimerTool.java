package sumscope.lava.mq;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.TimerTask;
import java.util.concurrent.atomic.AtomicInteger;

public class TimerTool extends TimerTask {

    private final Logger logger = LoggerFactory.getLogger(this.getClass());
    private static int seconds = 2;
    private int mode = 0;

    static public AtomicInteger srvPushCnt = new AtomicInteger(0);
    static public AtomicInteger srvRspCnt = new AtomicInteger(0);
    static public AtomicInteger cliAsyncRecvCnt = new AtomicInteger(0);
    static public AtomicInteger cliSyncRecvCnt = new AtomicInteger(0);
    static public AtomicInteger cliSubsRecvCnt = new AtomicInteger(0);

    //Statistics mode
    //0: server push, 1: server response, 2: client async receive
    //3: client sync receive, 4: client subscribe receive
    public void setMode(int md) {
        mode = md;
    }

    private void printResult() {
        switch (mode) {
            case 0:
                logger.info("server push speed (per sec): {}",
                        srvPushCnt.getAndSet(0) / seconds);
                break;
            case 1:
                logger.info("server response speed (per sec): {}",
                        srvRspCnt.getAndSet(0) / seconds);
                break;
            case 2:
                logger.info("client receive async response data (per sec): {}",
                        cliAsyncRecvCnt.getAndSet(0) / seconds);
                break;
            case 3:
                logger.info("client receive sync response data (per sec): {}",
                        cliSyncRecvCnt.getAndSet(0) / seconds);
                break;
            case 4:
                logger.info("client receive subscribe response data (per sec): {}",
                        cliSubsRecvCnt.getAndSet(0) / seconds);
                break;
            default:
                logger.info("mode error!");
        }
    }

    public void run() {
        while (true) {
            printResult();

            try {
                Thread.sleep((long)seconds * 1000);
            } catch (Exception e) {
            }
        }
    }
}