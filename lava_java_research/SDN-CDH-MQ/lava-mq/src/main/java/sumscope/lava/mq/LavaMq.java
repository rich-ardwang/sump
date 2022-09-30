package sumscope.lava.mq;

import org.apache.commons.lang3.tuple.Pair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.Consumer;
import java.util.function.UnaryOperator;
import sumscope.lava.*;

public class LavaMq implements LavaBaseMq {
    //lava dlls loading
    static {
        try {
            System.loadLibrary("lava_box");
            System.out.println("Load lava_box libs ok.");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Native code library loading failed! Error:" + e);
            System.exit(1);
        }
    }

    private static Logger logger = LoggerFactory.getLogger(LavaMq.class);
    private static ConcurrentHashMap<String, String> hashServerAddrMap =
            new ConcurrentHashMap<>();
    private static ConcurrentHashMap<String, LavaServer> sharedLavaSvrMap =
            new ConcurrentHashMap<>();
    private static ConcurrentHashMap<String, Integer> sharedLavaSvrRefCntMap =
            new ConcurrentHashMap<>();
    private static LavaConfig lavaConf = new LavaConfig();
    private static boolean lavaEnvInit = false;

    private String url = "";
    private String topicOrQueue = "";
    private String svrImageWildTopic = "";
    private boolean running = false;
    private boolean init = false;
    private boolean asClient = false;
    private boolean asServer = false;
    private LavaClient lavaClient = null;
    private LavaServer lavaServer = null;
    private byte[] subRequest = null;
    private Object subConsumer = null;
    private boolean subOnly = false;
    private boolean isSubscribe = false;
    private UnaryOperator<Pair<String, byte[]>> svrResponseOpt = null;
    private UnaryOperator<Pair<String, byte[]>> svrSubImageHandler = null;

/*------------------------------------private functions------------------------------------------------*/

    private void createClient(String url) {
        lavaClient = new LavaClient(url, lavaConf, this);
        this.url = url;
    }

    private boolean canCreateServer() {
        String serverAddr = lavaConf.getHost() + ":" + lavaConf.getPort();
        if (hashServerAddrMap.containsKey(serverAddr)) {
            logger.error("Lava server binding [{}] already exist, can't create again!", serverAddr);
            return false;
        } else {
            return true;
        }
    }

    private void regServerByUrl(String url) {
        String serverAddr = lavaConf.getHost() + ":" + lavaConf.getPort();
        if (!hashServerAddrMap.containsKey(serverAddr)) {
            hashServerAddrMap.put(serverAddr, url);
        }
    }

    private void unregServerByUrl(String url) {
        Set<String> addrSet = hashServerAddrMap.keySet();
        for (String addr : addrSet) {
            if (url.equals(hashServerAddrMap.get(addr))) {
                hashServerAddrMap.remove(addr);
            }
        }
    }

    private boolean createServer(String url) {
        if (sharedLavaSvrMap.containsKey(url)) {
            lavaServer = sharedLavaSvrMap.get(url);
        } else {
            if (canCreateServer()) {
                lavaServer = new LavaServer(url, lavaConf);
                lavaServer.init();
                sharedLavaSvrMap.put(url, lavaServer);
                regServerByUrl(url);
            } else {
                return false;
            }
        }

        if (!svrImageWildTopic.isEmpty()) {
            lavaServer.regLavaMqImageServer(this, svrImageWildTopic);
        }

        if (sharedLavaSvrRefCntMap.containsKey(url)) {
            Integer refCnt = sharedLavaSvrRefCntMap.get(url);
            sharedLavaSvrRefCntMap.replace(url, refCnt++);
        } else {
            Integer refCnt = 1;
            sharedLavaSvrRefCntMap.put(url, refCnt);
        }

        this.url = url;
        this.init = true;
        return true;
    }

/*------------------------------------public functions------------------------------------------------*/
    public UnaryOperator<Pair<String, byte[]>> getLavaMqServerSubImageHandler() {
        return svrSubImageHandler;
    }

    public boolean getMqObjectRunState() {
        return running;
    }

/*------------------------------------override functions------------------------------------------------*/

    @Override
    public void initConfig(LavaConfig conf) {
        if (null == conf) {
            logger.error("initConfig failed! conf is null.");
            return;
        }
        try {
            lavaConf = (LavaConfig)conf.clone();
            if (!lavaEnvInit) {
                lava_box.init_environment(lavaConf.getClientIOThreads(), lavaConf.getServerWorkerThreads());
                logger.info("Lava environment init ok.");
                lavaEnvInit = true;
            }
        } catch (Exception e) {
            logger.error("Lava config clone failed! e:{}", e);
        }
    }

    @Override
    public void initClientSubscribeBytes(String url, String wildTopic,
                                         Consumer<Pair<String, byte[]>> subscribeConsumer) {
        if (init) {
            logger.debug("initClientSubscribeBytes already init.");
            return;
        }

        // check input params
        if (null == url) {
            logger.error("initClientSubscribeBytes failed! url is null.");
            return;
        }
        if (url.isEmpty()) {
            logger.error("initClientSubscribeBytes failed! url is empty.");
            return;
        }
        if (null == wildTopic) {
            logger.error("initClientSubscribeBytes failed! wildTopic is null.");
            return;
        }
        if (wildTopic.isEmpty()) {
            logger.error("initClientSubscribeBytes failed! wildTopic is empty.");
            return;
        }
        if (null == subscribeConsumer) {
            logger.error("initClientSubscribeBytes failed! subscribeConsumer is null.");
            return;
        }

        createClient(url);
        topicOrQueue = wildTopic;
        subConsumer = subscribeConsumer;
        subOnly = true;
        isSubscribe = true;
        asClient = true;
        init = true;
    }

    @Override
    public void initClientSubscribeImageBytes(String url, String topic, byte[] requestParams,
                                              Consumer<Pair<String, byte[]>> subscribeConsumer) {
        if (init) {
            logger.debug("initClientSubscribeImageBytes already init.");
            return;
        }

        // check input params
        if (null == url) {
            logger.error("initClientSubscribeImageBytes failed! url is null.");
            return;
        }
        if (url.isEmpty()) {
            logger.error("initClientSubscribeImageBytes failed! url is empty.");
            return;
        }
        if (null == topic) {
            logger.error("initClientSubscribeImageBytes failed! topic is null.");
            return;
        }
        if (topic.isEmpty()) {
            logger.error("initClientSubscribeImageBytes failed! topic is empty.");
            return;
        }
        if (null == requestParams) {
            logger.error("initClientSubscribeImageBytes failed! requestParams is null.");
            return;
        }
        if (null == subscribeConsumer) {
            logger.error("initClientSubscribeImageBytes failed! subscribeConsumer is null.");
            return;
        }

        createClient(url);
        topicOrQueue = topic;
        subRequest = requestParams;
        subConsumer = subscribeConsumer;
        isSubscribe = true;
        asClient = true;
        init = true;
    }

    @Override
    public void initClientRequest(String url) {
        if (init) {
            logger.debug("initClientRequest already init.");
            return;
        }

        // check input params
        if (null == url) {
            logger.error("initClientRequest failed! url is null.");
            return;
        }
        if (url.isEmpty()) {
            logger.error("initClientRequest failed! url is empty.");
            return;
        }

        createClient(url);
        asClient = true;
        init = true;
    }

    @Override
    public void initServerPublish(String url, String wildTopic, UnaryOperator<Pair<String, byte[]>> imageHandler) {
        if (init) {
            logger.debug("initServerPublish already init.");
            return;
        }

        // check input params
        if (null == url) {
            logger.error("initServerPublish failed! url is null.");
            return;
        }
        if (url.isEmpty()) {
            logger.error("initServerPublish failed! url is empty.");
            return;
        }
        if (null == wildTopic) {
            wildTopic = "";
        }
        if (wildTopic.isEmpty() && (null != imageHandler)) {
            logger.error("initServerPublish failed! wildTopic and imageHandler must be the same empty or not.");
            return;
        }
        if (!wildTopic.isEmpty() && (null == imageHandler)) {
            logger.error("initServerPublish failed! wildTopic and imageHandler must be the same empty or not.");
            return;
        }

        if (!wildTopic.isEmpty() && (null != imageHandler)) {
            svrSubImageHandler = imageHandler;
        }
        svrImageWildTopic = wildTopic;
        createServer(url);
        asServer = true;
    }

    @Override
    public void initServerResponseBytes(String url, String queue,
                                        UnaryOperator<Pair<String, byte[]>> bytesResponseHandler) {
        if (init) {
            logger.debug("initServerResponseBytes already init.");
            return;
        }

        // check input params
        if (null == url) {
            logger.error("initServerResponseBytes failed! url is null.");
            return;
        }
        if (url.isEmpty()) {
            logger.error("initServerResponseBytes failed! url is empty.");
            return;
        }
        if (null == queue) {
            logger.error("initServerResponseBytes failed! queue is null.");
            return;
        }
        if (queue.isEmpty()) {
            logger.error("initServerResponseBytes failed! queue is empty.");
            return;
        }
        if (null == bytesResponseHandler) {
            logger.error("initServerResponseBytes failed! bytesResponseHandler is null.");
            return;
        }

        if (createServer(url)) {
            topicOrQueue = queue;
            svrResponseOpt = bytesResponseHandler;
        }
        asServer = true;
    }

    @Override
    public void setServerEventListener(Consumer<Pair<Integer, String>> serverEventConsumer) {
        if (null == serverEventConsumer || !asServer) {
            logger.error("setServerEventLisnter failed! serverEventConsumer is null, or it's not a server.");
            return;
        }
        lavaServer.setEventConsumer(serverEventConsumer);
    }

    @Override
    public void setClientEventListener(Consumer<Pair<Integer, String>> clientEventConsumer) {
        if (null == clientEventConsumer || !asClient) {
            logger.error("setClientEventLisnter failed! clientEventConsumer is null, or it's not a client.");
            return;
        }
        lavaClient.setEventConsumer(clientEventConsumer);
    }

    @Override
    public void start() {
        if (!init) {
            logger.error("Start failed! Not init.");
            return;
        }
        if (running) {
            logger.debug("Already start.");
            return;
        }

        if (asClient) {
            if (!lavaClient.isValidProxyPtr()) {
                lavaClient.createProxy();
            }
            if (isSubscribe) {
                lavaClient.subscribe(topicOrQueue, subRequest, subConsumer, subOnly);
            }
            logger.info("Lava mq client start.");
        }

        if (asServer) {
            if (!lavaServer.start()) {
                logger.error("Start lava server failed!");
                return;
            }
            //server response
            if (null != svrResponseOpt) {
                lavaServer.registerRequestHandler(topicOrQueue, svrResponseOpt);
            }
            logger.info("Lava mq server start.");
        }

        running = true;
    }

    @Override
    public boolean isRunning() {
        logger.info("Lava mq object is running.");
        return running;
    }

    @Override
    public void stop() {
        if (!init) {
            logger.error("Stop failed! Not init.");
            return;
        }
        if (!running) {
            logger.debug("Already running.");
            return;
        }

        if (asClient) {
            if (isSubscribe) {
                lavaClient.unsubscribe(topicOrQueue, subOnly);
            }
            logger.info("Lava mq client stop.");
        }

        if (asServer) {
            if (null != svrResponseOpt) {
                lavaServer.removeRequestHandler(topicOrQueue);
            }
            logger.info("Lava mq server stop.");
        }

        running = false;
    }

    @Override
    public void publishBytes(String msgid, String topic, byte[] bytes) {
        if (!running) {
            logger.warn("Lava server not running! Can't publish bytes!");
            return;
        }

        // check input params
        if (null == msgid) {
            logger.warn("publishBytes: msgid is null!");
            return;
        }
        if (msgid.isEmpty()) {
            logger.warn("publishBytes: msgid is empty!");
            return;
        }
        if (null == topic) {
            logger.warn("publishBytes: topic is null!");
            return;
        }
        if (topic.isEmpty()) {
            logger.warn("publishBytes: topic is empty!");
            return;
        }
        if (null == bytes) {
            logger.warn("publishBytes: bytes null!");
            return;
        }

        lavaServer.publishBytesMsg(topic, msgid, bytes);
    }

    @Override
    public void requestBytesAsync(String queue, byte[] requestParams, Consumer<byte[]> bytesReplyConsumer) {
        if (!running) {
            logger.warn("Lava client not running! Can't request bytes async!");
            return;
        }

        // check input params
        if (null == queue) {
            logger.warn("requestBytesAsync: Wrong input params!");
            return;
        }
        if (queue.isEmpty() || (null == requestParams) || (null == bytesReplyConsumer)) {
            logger.warn("requestBytesAsync: Wrong input params!");
            return;
        }

        try {
            lavaClient.requestAsync(queue, requestParams, bytesReplyConsumer);
        } catch (Exception e) {
            logger.error("requestBytesAsync unexpected!");
        }
    }

    @Override
    public byte[] requestBytesSync(String queue, byte[] requestParams, int timeoutInMs) {
        if (!running) {
            logger.warn("Lava client not running! Can't request bytes sync!");
            return null;
        }

        // check input params
        if (null == queue) {
            logger.warn("requestBytesSync: Wrong input params!");
            return null;
        }
        if (queue.isEmpty() || (null == requestParams) || (timeoutInMs < 0)) {
            logger.warn("requestBytesSync: Wrong input params!");
            return null;
        }

        try {
            return lavaClient.requestSync(queue, requestParams, timeoutInMs);
        } catch (Exception e) {
            logger.error("requestBytesSync unexpected!");
        }
        return null;
    }

    @Override
    public void releaseLavaMq() {
        if (!init || running) {
            logger.warn("Can't release lava mq object, not init or is running!");
            return;
        }
        if (asClient) {
            lavaClient.releaseLavaClient();
            logger.info("Release lava client success.");
        }
        if (asServer) {
            if (sharedLavaSvrRefCntMap.containsKey(url)) {
                lavaServer.unregLavaMqImageServer(this);
                Integer refCnt = sharedLavaSvrRefCntMap.get(url);
                refCnt--;
                if (0 == refCnt) {
                    sharedLavaSvrRefCntMap.remove(url);
                    lavaServer = sharedLavaSvrMap.get(url);
                    lavaServer.releaseLavaServer();
                    sharedLavaSvrMap.remove(url);
                    unregServerByUrl(url);
                    logger.info("Release lava server success, url:{}.", url);
                } else {
                    sharedLavaSvrRefCntMap.replace(url, refCnt);
                    logger.info("Decrease lava server reference count, refCnt:{}.", refCnt);
                }
            } else {
                logger.warn("Not find lava server by url, url:{}.", url);
            }
        }
    }
}
