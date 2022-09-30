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

public class LavaMqCdhTest2 {

    private static Logger logger = LoggerFactory.getLogger(LavaMqCdhTest.class);

    private static String SERVER_KEY = "SERVER.LAVA.CDH_TEST_SERVERA";
    private static String SERVER_KEYB = "SERVER.LAVA.CDH_TEST_SERVERB";
    private static String OUTPUT_TOPIC_PREFIX_A = "test.pub.A";
    private static String OUTPUT_TOPIC_PREFIX_B = "test.pub.B";
    private static String OUTPUT_TOPIC_PREFIX_C = "test.pub.C.*";
    private static String OUTPUT_TOPIC_PREFIX_C_18 = "test.pub.C.18";
    private static String OUTPUT_TOPIC_PREFIX_C_27 = "test.pub.C.27";

    private LavaBaseMq imgPub1 = LavaBaseMq.createLavaMq();
    private LavaBaseMq imgPub2 = LavaBaseMq.createLavaMq();
    private LavaBaseMq imgPub3 = LavaBaseMq.createLavaMq();
    private LavaBaseMq imgPub4 = LavaBaseMq.createLavaMq();

    public void initServer() {
        LavaConfig config = new LavaConfig();
        config.setNodeInfo("172.16.97.115:2181,172.16.97.116:2181,172.16.97.139:2181");
        config.setHost("127.0.0.1");
        config.setPort("5001");
        config.setServiceType(1);

        imgPub1.initConfig(config);

        imgPub1.initServerPublish(SERVER_KEY, "test.pub.A.*",
                new UnaryOperator<Pair<String, byte[]>>() {
                    @Override
                    public Pair<String, byte[]> apply(Pair<String, byte[]> pair) {
                        return Pair.of(pair.getLeft(), "image from imgPub1".getBytes());
                    }
                });

        imgPub2.initServerPublish(SERVER_KEYB, "test.pub.B.*",
                new UnaryOperator<Pair<String, byte[]>>() {
                    @Override
                    public Pair<String, byte[]> apply(Pair<String, byte[]> pair) {
                        return Pair.of(pair.getLeft(), "image from imgPub2".getBytes());
                    }
                });

        imgPub3.initServerPublish(SERVER_KEY, "", null);
        imgPub4.initServerPublish(SERVER_KEY, null, null);

        imgPub1.setServerEventListener(new Consumer<Pair<Integer, String>>() {
            @Override
            public void accept(Pair<Integer, String> integerStringPair) {
                logger.info("imgPub1 - ServerEventLisnter: {}", integerStringPair);
            }
        });
    }

    public void startServer() {
        imgPub1.start();
        imgPub2.start();
        imgPub3.start();
        imgPub4.start();
        timerForGenerateMessage();
    }

    private volatile int outputData1 = 0;
    private volatile int outputData2 = 0;
    private volatile int outputData3 = 0;
    private volatile int outputData4 = 0;

    private void timerForGenerateMessage() {
        Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                pub1();
                pub2();
                pub3();
                pub4();
            }
        }, 1000, 1000);
    }

    private void pub1() {
        outputData1++;
        String msgId = "MessageSeq.A." + String.valueOf(outputData1);
        String msg = "topic_1 Message:" + msgId;
        String topic = OUTPUT_TOPIC_PREFIX_A + "." + outputData1;
        imgPub1.publishBytes(msgId, topic, msg.getBytes(StandardCharsets.UTF_8));
        logger.info("imgPub1 has published to '{}', message: {}", topic, msg);
    }

    private void pub2() {
        outputData2++;
        String msgId = "MessageSeq.B." + String.valueOf(outputData2);
        String msg = "topic_2 Message:" + msgId;
        String topic = OUTPUT_TOPIC_PREFIX_B + "." + outputData2;
        imgPub2.publishBytes(msgId, topic, msg.getBytes(StandardCharsets.UTF_8));
        logger.info("imgPub2 has published to '{}', message: {}", topic, msg);
    }

    private void pub3() {
        outputData3++;
        String msgId = "MessageSeq.C.18." + String.valueOf(outputData3);
        String msg = "topic_3 Message:" + msgId;
        imgPub3.publishBytes(msgId, OUTPUT_TOPIC_PREFIX_C_18, msg.getBytes(StandardCharsets.UTF_8));
        logger.info("imgPub3 has published to '{}', message: {}", OUTPUT_TOPIC_PREFIX_C_18, msg);
    }

    private void pub4() {
        outputData4++;
        String msgId = "MessageSeq.C.27." + String.valueOf(outputData4);
        String msg = "topic_4 Message:" + msgId;
        imgPub4.publishBytes(msgId, OUTPUT_TOPIC_PREFIX_C_27, msg.getBytes(StandardCharsets.UTF_8));
        logger.info("imgPub4 has published to '{}', message: {}", OUTPUT_TOPIC_PREFIX_C_27, msg);
    }

    //////////////// client code
    private LavaBaseMq lavaSub1 = LavaBaseMq.createLavaMq();
    private LavaBaseMq lavaSub2 = LavaBaseMq.createLavaMq();
    private LavaBaseMq lavaSub3 = LavaBaseMq.createLavaMq();
    private LavaBaseMq lavaSub4 = LavaBaseMq.createLavaMq();
    private LavaBaseMq lavaSub5 = LavaBaseMq.createLavaMq();

    private int TIMEOUT_MS = 1000*60;

    public void initClient() {
        LavaConfig config = new LavaConfig();
        config.setNodeInfo("172.16.97.115:2181,172.16.97.116:2181,172.16.97.139:2181");
        config.setHost("127.0.0.1");
        config.setPort("5002");
        config.setServiceType(1);
        lavaSub1.initConfig(config);

        String topic1 = OUTPUT_TOPIC_PREFIX_A + ".30";
        String topic2 = OUTPUT_TOPIC_PREFIX_B + ".32";
        String topic3 = OUTPUT_TOPIC_PREFIX_A + ".*";
        String topic4 = OUTPUT_TOPIC_PREFIX_B + ".*";
        String topic5 = OUTPUT_TOPIC_PREFIX_C;

        logger.info("lavaSub1 subscribe topic: ({})", topic1);
        logger.info("lavaSub2 subscribe topic: ({})", topic2);
        logger.info("lavaSub3 subscribe topic: ({})", topic3);
        logger.info("lavaSub4 subscribe topic: ({})", topic4);
        logger.info("lavaSub5 subscribe topic: ({})", topic5);

        lavaSub1.initClientSubscribeImageBytes(SERVER_KEY, topic1, "not_used".getBytes(),
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> pair) {
                        logger.info("lavaSub1 got message, topic: {}, {}", pair.getLeft(),
                                StringUtils.left(new String(pair.getRight()), 500));
                    }
                });
        lavaSub2.initClientSubscribeImageBytes(SERVER_KEY, topic2, "not_used".getBytes(),
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> pair) {
                        logger.info("lavaSub2 got message, topic: {}, {}", pair.getLeft(),
                                StringUtils.left(new String(pair.getRight()), 500));
                    }
                });

        lavaSub3.initClientSubscribeBytes(SERVER_KEY, topic3,
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> pair) {
                        logger.info("lavaSub3 got message, topic: {}, {}", pair.getLeft(),
                                StringUtils.left(new String(pair.getRight()), 500));
                    }
                });
        lavaSub4.initClientSubscribeBytes(SERVER_KEY, topic4,
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> pair) {
                        logger.info("lavaSub4 got message, topic: {}, {}", pair.getLeft(),
                                StringUtils.left(new String(pair.getRight()), 500));
                    }
                });
        lavaSub5.initClientSubscribeBytes(SERVER_KEY, topic5,
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> pair) {
                        logger.info("lavaSub5 got message, topic: {}, {}", pair.getLeft(),
                                StringUtils.left(new String(pair.getRight()), 500));
                    }
                });
    }

    public void startClient() {
        int cnt3 = 0;
        int cnt4 = 0;
        int cnt5 = 0;
        mqStart(lavaSub1, "lavaSub1");
        mqStart(lavaSub2, "lavaSub2");
        mqStart(lavaSub3, "lavaSub3");
        mqStart(lavaSub4, "lavaSub4");
        mqStart(lavaSub5, "lavaSub5");
        while (true) {
            try {
                Thread.sleep((long)(1000 * 2));
                logger.info("sleep two second");
                cnt3++;
                cnt4++;
                cnt5++;
                if (10 == cnt3) {
                    mqStop(lavaSub3, "lavaSub3");
                }
                if (15 == cnt5) {
                    mqStop(lavaSub5, "lavaSub5");
                }
                if (20 == cnt4) {
                    mqStop(lavaSub4, "lavaSub4");
                }

                if (30 == cnt3) {
                    mqStart(lavaSub3, "lavaSub3");
                }
                if (35 == cnt5) {
                    mqStart(lavaSub5, "lavaSub5");
                }
                if (40 == cnt4) {
                    mqStart(lavaSub4, "lavaSub4");
                }
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
        logger.info("{} - start complete.", mqKey);
    }

    void mqStop(LavaBaseMq mq, String mqKey) {
        mq.stop();
        logger.info("{} - stop complete.", mqKey);
    }
}
