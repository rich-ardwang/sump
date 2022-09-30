package sumscope.lava.test;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.tuple.Pair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import sumscope.lava.mq.LavaBaseMq;
import sumscope.lava.mq.LavaConfig;

import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.function.Consumer;
import java.util.function.UnaryOperator;

public class LavaMqCdhTest {

    private static Logger logger = LoggerFactory.getLogger(LavaMqCdhTest.class);

    private static String SERVER_KEY = "SERVER.LAVA.CDH_TEST_SERVER";
    private static String OUTPUT_TOPIC = "test.output.pub";
    private static String REPLY_TOPIC = "test.reply.pub";

    private LavaBaseMq outputPubImageServer = LavaBaseMq.createLavaMq();
    private LavaBaseMq replyPubServer = LavaBaseMq.createLavaMq();

    public void initServer() {
        LavaConfig config = new LavaConfig();
        config.setNodeInfo("172.16.97.115:2181,172.16.97.116:2181,172.16.97.139:2181");
        config.setHost("127.0.0.1");
        config.setPort("5001");
        config.setServiceType(1);

        outputPubImageServer.initConfig(config);

        // 下面两行使用第一行就收不到, 使用第二行则没有问题
        //outputPubImageServer.initServerPublish(SERVER_KEY, null);
        outputPubImageServer.initServerPublish(SERVER_KEY, "",
                new UnaryOperator<Pair<String, byte[]>>() {
                    @Override
                    public Pair<String, byte[]> apply(Pair<String, byte[]> pair) {
                        return Pair.of(pair.getLeft(), "replies anything".getBytes());
                    }
                });

        replyPubServer.initServerPublish(SERVER_KEY, null, null);

        replyPubServer.setServerEventListener(new Consumer<Pair<Integer, String>>() {
            @Override
            public void accept(Pair<Integer, String> pair) {
                logger.warn("ServerEventLisnter: {}", pair);
                if (pair.getLeft().intValue() == 20) {
                    if (StringUtils.contains(pair.getRight(), REPLY_TOPIC)) {
                        replyPublish();
                    }
                }
            }
        });
    }

    public void startServer() {
        outputPubImageServer.start();
        replyPubServer.start();
        timerForGenerateMessage();
    }

    private volatile int outputIntData = 0;
    private volatile int replyIntData = 0;

    private void timerForGenerateMessage() {
        Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                outputIntData++;
                String msgId = "MessageSeq." + String.valueOf(outputIntData);
                String msg = "Output Message:" + String.valueOf(outputIntData);
                outputPubImageServer.publishBytes(msgId, OUTPUT_TOPIC, msg.getBytes(StandardCharsets.UTF_8));
                logger.info("has published to '{}', message: {}", OUTPUT_TOPIC, msg);
            }
        }, 1000, 1000);
    }

    private void replyPublish() {
        for(int i=0; i<1000; i++) {
            replyIntData--;
            String msgId = "MessageSeq." + String.valueOf(replyIntData);
            String msg = "Reply Message:" + String.valueOf(replyIntData);
            replyPubServer.publishBytes(msgId, REPLY_TOPIC, msg.getBytes(StandardCharsets.UTF_8));
            logger.info("has published to '{}', message: {}", REPLY_TOPIC, msg);
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    //////////////// client code

    private LavaBaseMq lavaSubOutput = LavaBaseMq.createLavaMq();
    private LavaBaseMq lavaSubReply = LavaBaseMq.createLavaMq();

    private int TIMEOUT_MS = 1000*60;

    public void initClient() {

        LavaConfig config = new LavaConfig();
        config.setNodeInfo("172.16.97.115:2181,172.16.97.116:2181,172.16.97.139:2181");
        config.setHost("127.0.0.1");
        config.setPort("5002");
        config.setServiceType(1);
        lavaSubOutput.initConfig(config);

        // 下面两行使用第一行就收不到, 使用第二行则没有问题
        //lavaSubOutput.initClientSubscribeBytes(SERVER_KEY, OUTPUT_TOPIC,
        lavaSubOutput.initClientSubscribeImageBytes(SERVER_KEY, OUTPUT_TOPIC, "not_used".getBytes(),

                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> pair) {
                        logger.info("got message, topic: {}, {}", pair.getLeft(),
                                StringUtils.left(new String(pair.getRight()), 500));
                    }
                });

        lavaSubReply.initClientSubscribeBytes(SERVER_KEY, REPLY_TOPIC,
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> pair) {
                        logger.info("got message, topic: {}, {}", pair.getLeft(),
                                StringUtils.left(new String(pair.getRight()), 500));
                    }
                });
    }

    public void startClient() {
        mqStart(lavaSubOutput, "lavaSubOutput");
        mqStart(lavaSubReply, "lavaSubReply");

        while (true) {
            try {
                Thread.sleep((long)(1000 * 2));
                logger.info("sleep one second");
            } catch (Exception e) {
            }
        }
    }

    void mqStart(LavaBaseMq mq, String mqKey) {
        mq.setClientEventListener(new Consumer<Pair<Integer, String>>() {
            @Override
            public void accept(Pair<Integer, String> integerStringPair) {
                logger.info("{} - ClientEventLisnter: {}", mqKey, integerStringPair);
            }
        });
        mq.start();
    }
}
