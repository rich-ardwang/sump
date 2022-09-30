package sumscope.lava.test;

import org.apache.commons.configuration.PropertiesConfiguration;
import org.apache.commons.lang3.tuple.Pair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import sumscope.lava.mq.LavaBaseMq;
import sumscope.lava.mq.TimerTool;
import sumscope.lava.mq.LavaConfig;
import java.util.Random;
import java.util.Timer;
import java.util.function.Consumer;
import java.util.function.UnaryOperator;

public class LavaMqTest {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());
    private PropertiesConfiguration testConfig = null;
    private String url;
    private String asyncQueue;
    private String syncQueue;
    private String asyncServerWildQueue;
    private String syncServerWildQueue;
    private String subWildTopic;
    private String subImageTopic;
    private String publishTopic;
    private String publishmageTopic;
    private int timeout;

    //生成指定length的随机字符串（A-Z，a-z，0-9）
    private String getRandomString(int length) {
        String str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        Random random = new Random();
        StringBuffer sbf = new StringBuffer();
        for (int i = 0; i < length; i++) {
            int number = random.nextInt(str.length());
            sbf.append(str.charAt(number));
        }
        return sbf.toString();
    }

    public LavaMqTest() {
        //load config files
        try {
            //init lava mq config
            LavaBaseMq inst = LavaBaseMq.createLavaMq();
            LavaConfig conf = new LavaConfig();

            //init test items
            testConfig = new PropertiesConfiguration("./control/LavaMqConf.ini");
            String[] zookeeperArray = testConfig.getStringArray("Test.zookeeper");
            String zookeeper = "";
            int cnt = 0;
            for (String tmp : zookeeperArray) {
                cnt++;
                zookeeper += tmp;
                if (cnt != zookeeperArray.length) {
                    zookeeper += ",";
                }
            }
            if (zookeeper.isEmpty()) {
                zookeeper = "172.16.97.115:2181,172.16.97.116:2181,172.16.97.139:2181";
            }

            String host = testConfig.getString("Test.server.ip", "127.0.0.1");
            String port = testConfig.getString("Test.server.port", "10001");
            String shareSession = testConfig.getString("Test.client.shareSession", "0");
            conf.setNodeInfo(zookeeper);
            conf.setHost(host);
            conf.setPort(port);
            if (shareSession.equals("0")) {
                conf.setShareSession(false);
            } else {
                conf.setShareSession(true);
            }
            inst.initConfig(conf);

            url = testConfig.getString("Test.url", "SDN_CDH_LAVA_DEMO");
            asyncQueue = testConfig.getString("Test.client.async.request.queue", "sdn.cdh.demo.lava.req.A");
            syncQueue = testConfig.getString("Test.client.sync.request.queue", "sdn.cdh.demo.lava.req.B");
            asyncServerWildQueue = testConfig.getString("Test.server.async.response.wildqueue", "sdn.cdh.demo.*.req.A");
            syncServerWildQueue = testConfig.getString("Test.server.sync.response.wildqueue", "sdn.cdh.**.req.B");
            subWildTopic = testConfig.getString("Test.client.subscribe.wildTopic", "sdn.cdh.demo.lava.*");
            subImageTopic = testConfig.getString("Test.client.subscribe.image.topic", "sdn.cdh.demo.lava.8");
            publishTopic = testConfig.getString("Test.server.publish.topic", "sdn.cdh.demo.lava.8");
            publishmageTopic = testConfig.getString("Test.server.publish.image.topic", "sdn.cdh.demo.lava.*");
            timeout = testConfig.getInt("Test.sync.timeout", 3000);
        } catch (Exception e) {
            logger.error("load lava mq config failed!");
        }
    }

    //1.Async request (client)
    public void clientAsyncTest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(2);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq req = LavaBaseMq.createLavaMq();
        req.initClientRequest(url);
        req.start();
        String reqParams = "lava mq client request: [Async request].";
        //String reqParams = getRandomString(8*1024*1024);
        //int cnt = 0;
        while (true) {
            //already test ok, so comment it
            /*
            try {
                cnt++;
                if (0 == (cnt % 10000)) {
                    req.stop();
                    logger.info("client async stop.");

                    Thread.sleep((long)(1000 * 10));
                    req.start();
                    logger.info("client start again.");
                }
                if (2*10000 == cnt) {
                    req.stop();
                    logger.info("client stop.");
                    req.releaseLavaMq();
                    logger.info("client release.");
                    break;
                }
            } catch(Exception e) {
            }
            */

            req.requestBytesAsync(asyncQueue, reqParams.getBytes(),
                    new Consumer<byte[]>() {
                        @Override
                        public void accept(byte[] rspData) {
                            if (mode) {
                                task.cliAsyncRecvCnt.getAndIncrement();
                            } else {
                                logger.info("client receive response data of async: {}.", rspData);

                                try {
                                    Thread.sleep((long)(1000 * 2));
                                } catch (Exception e) {
                                }
                            }
                        }
                    });
        }
    }

    //2.Sync request (client)
    public void clientSyncTest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(3);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq req = LavaBaseMq.createLavaMq();
        req.initClientRequest(url);
        req.start();
        String reqParams = "lava mq client request: [Sync request].";
        //String reqParams = getRandomString(8*1024*1024);
        byte[] rspData = null;
        while (true) {
            rspData = req.requestBytesSync(syncQueue,
                    reqParams.getBytes(), timeout);
            if (mode) {
                task.cliSyncRecvCnt.getAndIncrement();
            } else {
                logger.info("client receive response data of sync: {}.", rspData);

                try {
                    Thread.sleep((long)(1000 * 2));
                } catch (Exception e) {
                }
            }
        }
    }

    //3.Response TypeA (server)
    public void serverResponseATest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(1);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq response = LavaBaseMq.createLavaMq();
        response.initServerResponseBytes(url, asyncServerWildQueue,
                new UnaryOperator<Pair<String, byte[]>>() {
                    @Override
                    public Pair<String, byte[]> apply(Pair<String, byte[]> reqInfo) {
                        if (mode) {
                            task.srvRspCnt.getAndIncrement();
                        } else {
                            logger.info("server receive the request from client, topic: {}, reqParam: {}.",
                                    reqInfo.getLeft(), reqInfo.getRight());
                        }
                        String rspData = "this is lava server response A.";
                        //String rspData = getRandomString(8*1024*1024);
                        Pair<String, byte[]> rspPair = Pair.of(reqInfo.getLeft(), rspData.getBytes());
                        while (true) {
                            try {
                                Thread.sleep((long)(1000 * 2));
                            } catch (Exception e) {
                            }
                        }
                        //return rspPair;
                    }
                });
        response.start();

        //int cnt = 0;
        while (true) {
            try {
                Thread.sleep((long)(1000 * 2));
                //already test ok, so comment it
                /*
                cnt++;
                if (0 == (cnt % 10)) {
                    response.stop();
                    logger.info("server response stop.");

                    Thread.sleep((long)(1000 * 10));
                    response.start();
                    logger.info("server start again.");
                }
                if (25 == cnt) {
                    response.stop();
                    logger.info("server stop.");
                    response.releaseLavaMq();
                    logger.info("server release.");
                    break;
                }
                */
            } catch (Exception e) {
            }
        }
    }

    //4.Response TypeB(server)
    public void serverResponseBTest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(1);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq responseB = LavaBaseMq.createLavaMq();
        responseB.initServerResponseBytes(url, syncServerWildQueue,
                new UnaryOperator<Pair<String, byte[]>>() {
                    @Override
                    public Pair<String, byte[]> apply(Pair<String, byte[]> reqInfo) {
                        if (mode) {
                            task.srvRspCnt.getAndIncrement();
                        } else {
                            logger.info("server receive the request from client, topic: {}, reqParam: {}.",
                                    reqInfo.getLeft(), reqInfo.getRight());
                        }
                        String rspData = "this is lava server response B.";
                        //String rspData = getRandomString(8*1024*1024);
                        Pair<String, byte[]> rspPair = Pair.of(reqInfo.getLeft(), rspData.getBytes());
                        return rspPair;
                    }
                });
        responseB.start();

        while (true) {
            try {
                Thread.sleep((long)(1000 * 2));
            } catch (Exception e) {
            }
        }
    }

    //5.Subscribe(client)
    public void clientSubscribeTest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(4);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq client = LavaBaseMq.createLavaMq();
        client.initClientSubscribeBytes(url, subWildTopic,
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> msg) {
                        if (mode) {
                            task.cliSubsRecvCnt.getAndIncrement();
                        } else {
                            logger.info("client receive the response of subscribe: topic: {}, content: {}.",
                                    msg.getLeft(),
                                    msg.getRight());
                        }
                    }
                });
        client.start();

        //int cnt = 0;
        while (true) {
            try {
                Thread.sleep((long)(1000 * 2));
                //already test ok, so comment it
                /*
                cnt++;
                if (0 == (cnt % 10)) {
                    client.stop();
                    logger.info("client stop.");

                    Thread.sleep((long)(1000 * 10));
                    client.start();
                    logger.info("client start again.");
                }
                if (25 == cnt) {
                    client.stop();
                    logger.info("client stop.");
                    client.releaseLavaMq();
                    logger.info("client release.");
                    break;
                }
                */
            } catch (Exception e) {
            }
        }
    }

    //6.Subscribe with image(client)
    public void clientSubscribeImageTest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(4);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq client = LavaBaseMq.createLavaMq();
        String reqParam = "this is client image request param.";
        client.initClientSubscribeImageBytes(url, subImageTopic,
                reqParam.getBytes(), new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> msg) {
                        if (mode) {
                            task.cliSubsRecvCnt.getAndIncrement();
                        } else {
                            logger.info("client receive the response of subscribe: topic: {}, content: {}.",
                                    msg.getLeft(),
                                    msg.getRight());
                        }
                    }
                });
        client.setClientEventListener(new Consumer<Pair<Integer, String>>() {
            @Override
            public void accept(Pair<Integer, String> msg) {
                logger.info("client receive the event: event_id: {}, event_msg: {}.",
                        msg.getLeft(),
                        msg.getRight());
            }
        });
        client.start();

        while (true) {
            try {
                Thread.sleep((long)(1000 * 2));
            } catch (Exception e) {
            }
        }
    }

    //7.publish subscribe(server)
    public void serverSubscribeTest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(0);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq server = LavaBaseMq.createLavaMq();
        server.initServerPublish(url, null, null);
        server.start();

        int id = 1;
        String publishData = "This is server's response for subscribe.";
        //int cnt = 0;
        while (true) {
            try {
                //already test ok, so comment it
                /*
                cnt++;
                if (0 == (cnt % 100000)) {
                    server.stop();
                    logger.info("server push stop.");

                    Thread.sleep((long)(1000 * 10));
                    server.start();
                    logger.info("server start again.");
                }
                if (2*100000 == cnt) {
                    server.stop();
                    logger.info("server stop.");
                    server.releaseLavaMq();
                    logger.info("server release.");
                    break;
                }
                */

                server.publishBytes(Integer.toString(id), publishTopic, publishData.getBytes());
                if (!mode) {
                    logger.info("server push: msgid: {}, topic: {}, content: {}.",
                            Integer.toString(id), publishTopic, publishData);
                    Thread.sleep((long)(1000));
                } else {
                    task.srvPushCnt.getAndIncrement();
                }
                id++;
            } catch (Exception e) {
            }
        }
    }

    //8.publish subscribe image(server)
    public void serverSubscribeImageTest(boolean mode) {
        TimerTool task = new TimerTool();
        if (mode) {
            task.setMode(0);
            Timer timer = new Timer();
            timer.schedule(task, 0);
        }
        LavaBaseMq server = LavaBaseMq.createLavaMq();
        server.initServerPublish(url, publishmageTopic, new UnaryOperator<Pair<String, byte[]>>() {
                @Override
                public Pair<String, byte[]> apply(Pair<String, byte[]> reqImageInfo) {
                    logger.info("server receive the request: topic: {}, image params: {}.",
                            reqImageInfo.getLeft(), reqImageInfo.getRight());
                    /*
                    String imageData = "China hails investment from Japanese and South Korean entrepreneurs and will treat them equally with identical protection and opportunities, said Premier Li Keqiang on the sidelines of a trilateral summit held on Dec 24.\n" +
                            "\n" +
                            "Premier Li made the statement while delivering a speech at the opening ceremony of the 7th China-Japan-ROK Business Summit in Chengdu, Southwest China's Sichuan province, with Prime Minister Shinzo Abe of Japan and President Moon Jae-in of the Republic of Korea (ROK) in attendance.\n" +
                            "\n" +
                            "Premier Li said all the businesses registered in China will be taken as homegrown enterprises and encouraged Japanese and South Korean entrepreneurs to grasp the opportunity by occupying a spot in the country's resilient economy.";
                     */
                    String imageData = getRandomString(1200);
                    Pair<String, byte[]> retPairData = Pair.of(reqImageInfo.getLeft(), imageData.getBytes());
                    /*
                    while (true) {
                        try {
                            Thread.sleep((long)(5000));
                        } catch (Exception e) {
                        }
                    }
                    */
                    return retPairData;
                }
            });
        server.setServerEventListener(new Consumer<Pair<Integer, String>>() {
                     @Override
                     public void accept(Pair<Integer, String> msg) {
                         logger.info("server receive the event: event_id: {}, event_msg: {}.",
                                 msg.getLeft(),
                                 msg.getRight());
                     }
                 });
        server.start();

        int id = 1;
        String publishData = "This is server's response for subscribe with image.";
        while (true) {
            try {
                server.publishBytes(Integer.toString(id), publishTopic, publishData.getBytes());
                if (!mode) {
                    logger.info("server push: msgid: {}, topic: {}, content: {}.",
                            Integer.toString(id), publishTopic, publishData);
                    Thread.sleep((long)(1000));
                } else {
                    task.srvPushCnt.getAndIncrement();
                }
                id++;
            } catch (Exception e) {
            }
        }
    }

    //9.(serverE)
    public void MultiServerMqTest() {
        //server mq take charge for response
        LavaBaseMq responseServer = LavaBaseMq.createLavaMq();
        responseServer.initServerResponseBytes(url, asyncServerWildQueue,
                new UnaryOperator<Pair<String, byte[]>>() {
                    @Override
                    public Pair<String, byte[]> apply(Pair<String, byte[]> reqInfo) {
                        logger.info("server receive the request from client, topic: {}, reqParam: {}.",
                                    reqInfo.getLeft(), reqInfo.getRight());

                        String rspData = "server mq take charge for response.";
                        Pair<String, byte[]> rspPair = Pair.of(reqInfo.getLeft(), rspData.getBytes());
                        return rspPair;
                    }
                });
        responseServer.start();

        //server mq take charge for publish
        LavaBaseMq publishServer = LavaBaseMq.createLavaMq();
        publishServer.initServerPublish(url, null, null);
        publishServer.start();

        int id = 1;
        String publishData = "server mq take charge for publish.";
        while (true) {
            try {
                publishServer.publishBytes(Integer.toString(id), publishTopic, publishData.getBytes());
                logger.info("server push: msgid: {}, topic: {}, content: {}.",
                            Integer.toString(id), publishTopic, publishData);
                id++;
                Thread.sleep((long)(5000));
            } catch (Exception e) {
            }
        }
    }

    //10.(clientE)
    public void MultiClientMqTest() {
        //Client mq take charge for sub
        LavaBaseMq subClient = LavaBaseMq.createLavaMq();
        subClient.initClientSubscribeBytes(url, subWildTopic,
                new Consumer<Pair<String, byte[]>>() {
                    @Override
                    public void accept(Pair<String, byte[]> msg) {
                        logger.info("client receive the response of subscribe: topic: {}, content: {}.",
                                msg.getLeft(),
                                msg.getRight());

                        try {
                            Thread.sleep((long) (1000 * 3));
                        } catch (Exception e) {
                        }
                    }
        });
        subClient.setClientEventListener(new Consumer<Pair<Integer, String>>() {
            @Override
            public void accept(Pair<Integer, String> integerStringPair) {
                logger.info("{} - ClientEventLisnter: {}", "subClient", integerStringPair);
            }
        });
        subClient.start();

        /*
        while (true) {
            try {
                Thread.sleep((long)(1000 * 2));
            } catch (Exception e) {
            }
        }

         */

        //Client mq take charge for req
        LavaBaseMq reqClient = LavaBaseMq.createLavaMq();
        reqClient.initClientRequest(url);
        reqClient.start();
        String reqParams = "Client mq take charge for req.";
        while (true) {
            try {
                reqClient.requestBytesAsync(asyncQueue, reqParams.getBytes(),
                        new Consumer<byte[]>() {
                            @Override
                            public void accept(byte[] rspData) {
                                logger.info("client receive response data of async: {}.", rspData);
                                try {
                                    Thread.sleep((long) (1000 * 2));
                                } catch (Exception e) {
                                }
                            }
                        });
                Thread.sleep((long)(1000 * 3));
            } catch (Exception e) {
            }
        }


    }

}
