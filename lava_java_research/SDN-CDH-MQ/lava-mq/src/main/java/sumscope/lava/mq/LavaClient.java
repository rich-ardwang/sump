package sumscope.lava.mq;

import org.apache.commons.lang3.tuple.Pair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.nio.ByteBuffer;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;
import sumscope.lava.*;

public class LavaClient {
    //client event listener internal use
    private class ClientEventListener extends i_event_listener {
        private AtomicInteger count = new AtomicInteger(0);
        private LavaClient lavaClientObj = null;

        public ClientEventListener(LavaClient obj) {
            super();
            add_ref();
            lavaClientObj = obj;
        }

        /*---------------------------interfaces implementation--------------------------------------*/
        @Override
        public void on_event(int event_code, SWIGTYPE_p_void event_data) {
            String retMsg = "";
            // event_login_success : 8
            if (event_code == event_login_success) {
                synchronized (lavaClientObj.loginLock) {
                    lavaClientObj.loginLock.sinal = true;
                    lavaClientObj.loginLock.notify();
                }
                retMsg = "Login success.";
            } else if (event_code == event_conn_disconn) {
                // event_conn_disconn : 2
                retMsg = "Disconnected!";
            } else {
                //nothing to do
                logger.info("Received unrecognized event. EventCode:{}.", event_code);
                return;
            }
            if (null != cliEvnConsumer) {
                cliEvnConsumer.accept(Pair.of(event_code, retMsg));
                logger.info("Notify lava mq client listener. EventCode:{}, EventMsg:{}.", event_code, retMsg);
            }
        }

        @Override
        public void on_exception(int error_code, String what) {
            logger.error("Lava client event exception: err_code={}, what={}.", error_code, what);
        }

        @Override
        public int add_ref() {
            return count.incrementAndGet();
        }

        @Override
        public int release() {
            int ret = count.decrementAndGet();
            if ( ret <= 0 ) {
                this.delete();
                return 0;
            }
            return ret;
        }

        @Override
        public int reserved(SWIGTYPE_p_void arg0, SWIGTYPE_p_p_void arg1) {
            return 0;
        }
    }

    //client message listener internal use
    private class ClientMsgListener extends i_message_listener {
        private AtomicInteger count = new AtomicInteger(0);

        public ClientMsgListener() {
            super();
            add_ref();
        }

        /*---------------------------interfaces implementation--------------------------------------*/
        @Override
        public void on_message(i_message msg) {
            if (!lavaMqClientObj.getMqObjectRunState()) {
                logger.warn("on_message: Lava mq client object is not running. Discard the message!");
                return;
            }
            if (null == asyncRequestConsumer) {
                logger.error("on_message: asyncRequestConsumer is empty!");
                return;
            }
            if (null == msg) {
                logger.error("on_message: Receive null message!");
                return;
            }
            ByteBuffer nioBuf = msg.get_message_body().get_bytes(LavaMessageEnum.LAVA_MSG_REQUEST_BYTE);
            if (null == nioBuf) {
                logger.error("on_message: Server request buf is null!");
                return;
            }
            int bufLen = nioBuf.limit() - nioBuf.position();
            byte[] byteBuf = new byte[bufLen];
            nioBuf.get(byteBuf);
            asyncRequestConsumer.accept(byteBuf);
        }

        @Override
        public void on_reply(i_message msg, String topic) {
            if (!lavaMqClientObj.getMqObjectRunState()) {
                logger.warn("on_reply: Lava mq client object is not running. Discard the message!");
                return;
            }
            if (null == consumerInfo.subsConsumer) {
                logger.error("on_reply: subsConsumer is empty!");
                return;
            }
            if (null == msg) {
                logger.error("on_reply: Receive null message! topic:{}.", topic);
                return;
            }

            // handle sub reply message
            if (subImage) {
                if (!snapshotComplete) {
                    // get snapshot flag
                    String snapshotFlag = msg.get_message_body().get_string(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_MSGID);
                    if (imageSubFilter.equals(snapshotFlag)) {
                        // get rsp data and reply to client
                        ByteBuffer nioBuf = msg.get_message_body().get_bytes(LavaMessageEnum.LAVA_MSG_SUBS_RESPONSE_DATA);
                        if (null == nioBuf) {
                            logger.error("on_reply: Get snapshot bytes buf failed!");
                            return;
                        }
                        int bufLen = nioBuf.limit() - nioBuf.position();
                        byte[] byteBuf = new byte[bufLen];
                        nioBuf.get(byteBuf);
                        ((Consumer<Pair<String, byte[]>>) consumerInfo.subsConsumer).accept(
                                Pair.of(topic, byteBuf));
                        snapshotComplete = true;
                    } else {
                        // filter push message before the snapshot complete.
                        logger.debug("on_reply: Wait for snapshot.");
                    }
                    return;
                }
            } else {
                // filter image cache message
                long cacheFlag = msg.get_message_body().get_long(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_CACHE_MSGID);
                if (imageSubCacheFilter == cacheFlag) {
                    logger.debug("on_reply: This is image snapshot or cache messages, need not reply to mq client.");
                    return;
                }
            }

            //get rsp data and reply to client
            ByteBuffer nioBuf = msg.get_message_body().get_bytes(LavaMessageEnum.LAVA_MSG_SUBS_RESPONSE_DATA);
            if (null == nioBuf) {
                logger.error("on_reply: Get subscribe msg buf failed!");
                return;
            }
            int bufLen = nioBuf.limit() - nioBuf.position();
            byte[] byteBuf = new byte[bufLen];
            nioBuf.get(byteBuf);
            ((Consumer<Pair<String, byte[]>>) consumerInfo.subsConsumer).accept(
                    Pair.of(topic, byteBuf));
        }

        @Override
        public int add_ref() {
            return count.incrementAndGet();
        }

        @Override
        public int release() {
            int ret = count.decrementAndGet();
            if ( ret <= 0 ) {
                this.delete();
                return 0;
            }
            return ret;
        }

        @Override
        public int reserved(SWIGTYPE_p_void arg0, SWIGTYPE_p_p_void arg1) {
            return 0;
        }
    }

    class ConsumerInfo {
        public Object subsConsumer = null;
    }

    class LoginLock {
        public boolean sinal = false;
    }

    private static String imageSubFilter = "SUBS_IMAGE_FILTER";
    private static long imageSubCacheFilter = 99L;
    private Logger logger = LoggerFactory.getLogger(this.getClass());
    private String url = "";
    private boolean snapshotComplete = false;
    private boolean subImage = false;
    private ClientEventListener clientEventListener = null;
    private ClientMsgListener clientMsgListerner = null;
    private client_login_info lavaLoginInfo = null;
    private client_cluster_node_info cluster_info = null;
    private client_proxy_info pxy_info = null;
    private proxy_auto_ptr clientProxyPtr = null;
    private LoginLock loginLock = null;
    private ConsumerInfo consumerInfo = null;
    private long cookieAsyncRequest = 0;
    private Consumer<byte[]> asyncRequestConsumer = null;
    private LavaConfig lavaConfig = null;
    private Consumer<Pair<Integer, String>> cliEvnConsumer = null;
    private LavaMq lavaMqClientObj = null;

    public LavaClient(String url, LavaConfig conf, LavaMq obj) {
        this.url = url;
        lavaConfig = conf;
        lavaMqClientObj = obj;
        loginLock = new LoginLock();
        clientEventListener = new LavaClient.ClientEventListener(this);
        clientMsgListerner = new LavaClient.ClientMsgListener();
        setLavaLoginInfo();
        logger.info("Lava client create complete.");
    }

/*------------------------------------private functions------------------------------------------------*/

    private void setLavaLoginInfo() {
        lavaLoginInfo = new client_login_info();
        String serverAddr = lavaConfig.getHost() + ":" + lavaConfig.getPort();
        lavaLoginInfo.setM_host_port(serverAddr);
        logger.info("Lava client set server address: {}.", serverAddr);
        lavaLoginInfo.setM_username(lavaConfig.getUserName());
        lavaLoginInfo.setM_password(lavaConfig.getPassWord());
        lavaLoginInfo.setM_use_proxy(lavaConfig.isUseProxy());
        lavaLoginInfo.setM_use_cluster_mode(lavaConfig.isUseCluster());
        if (lavaLoginInfo.getM_use_proxy()) {
            pxy_info = new client_proxy_info();
            String proxyAddr = lavaConfig.getProxyHost() + ":" + lavaConfig.getProxyPort();
            pxy_info.setM_proxy_host_port(proxyAddr);
            switch (lavaConfig.getProxyType()) {
                case 1:
                    pxy_info.setM_proxy_type(proxy_type.proxy_http);
                    break;
                case 2:
                    pxy_info.setM_proxy_type(proxy_type.proxy_socks4);
                    break;
                case 3:
                    pxy_info.setM_proxy_type(proxy_type.proxy_socks5);
                    break;
                default:
                    pxy_info.setM_proxy_type(proxy_type.proxy_none);
                    break;
            }
            pxy_info.setM_proxy_username(lavaConfig.getProxyUsername());
            pxy_info.setM_proxy_password(lavaConfig.getProxyPassword());
            lavaLoginInfo.setM_proxy_info(pxy_info);
            logger.info("Lava client use proxy. Proxy address: {}, Proxy type: {}.", proxyAddr, lavaConfig.getProxyType());
        }
        if (lavaLoginInfo.getM_use_cluster_mode()) {
            cluster_info = new client_cluster_node_info();
            cluster_info.setM_cluster_type(cluster_type.cluster_zookeeper);
            cluster_info.setM_cluster_server_name(url);
            cluster_info.setM_cluster_nodes_info(lavaConfig.getNodeInfo());
            lavaLoginInfo.setM_cluster_info(cluster_info);
            logger.info("Lava client use cluster. Server URL: {}.", url);
        }
    }
    
    private void waitForClientConnected() {
        synchronized (loginLock) {
            try {
                logger.info("Waiting for lava connection to {}.", url);
                if (!loginLock.sinal) {
                    loginLock.wait(60 * 1000);
                }
            } catch (InterruptedException e) {
                logger.error("Waiting for lava connection 60 secs timeout!");
                clientProxyPtr.cancel_event(clientEventListener);
                e.printStackTrace();
            }
        }
        logger.info("Connected to server {} complete.", url);
    }

/*------------------------------------public functions------------------------------------------------*/

    public void createProxy() {
        try {
            client_cluster_node_info clusterInfo = lavaLoginInfo.getM_cluster_info();
            String serverName;
            if (null != clusterInfo) {
                serverName = clusterInfo.getM_cluster_server_name();
                if (serverName.isEmpty()) {
                    logger.error("[createProxy]: Get empty cluster server name!");
                    return;
                }
            } else {
                logger.error("[createProxy]: Get cluster info failed!");
                return;
            }
            clientProxyPtr = lava_box.create_proxy(serverName,
                    lavaLoginInfo, comm_protocol.tcp_comm, lavaConfig.isShareSession());
            if (null == clientProxyPtr) {
                logger.error("[createProxy]: Lava client create proxy failed!");
                return;
            }
            logger.info("Lava client create proxy success.");
            clientProxyPtr.set_auto_reconnect(lavaConfig.isAutoConnect(), lavaConfig.getTryTimes());
            clientProxyPtr.interest_event(clientEventListener);
            waitForClientConnected();
        } catch (Exception e) {
            logger.error("Lava client init failed! err={}", e);
        }
    }

    public boolean isValidProxyPtr() {
        if (null == clientProxyPtr) {
            return false;
        } else {
            return true;
        }
    }

    public void subscribe(String topic, byte[] requestParams, Object subscribeConsumer,
                          boolean subscribeOnly) {
        if (null == consumerInfo) {
            consumerInfo = new ConsumerInfo();
        }
        consumerInfo.subsConsumer = subscribeConsumer;

        //subscribe to lava box
        try {
            i_message msg = lava_box.create_message((short) lava_boxConstants.MSG_TYPE_LAVA_SUB);
            msg.set_topic(topic);
            if (subscribeOnly) {
                //0: subscribe 1: snapshot
                msg.get_message_body().add_int(LavaMessageEnum.LAVA_MSG_SUBS_TYPE, 0);
            } else {
                //0: subscribe 1: snapshot
                subImage = true;
                msg.get_message_body().add_int(LavaMessageEnum.LAVA_MSG_SUBS_TYPE, 1);

                //set request param for snapshot subs
                ByteBuffer reqBuf = msg.get_message_body().alloc_mem(requestParams.length);
                if (null ==  reqBuf) {
                    logger.error("Client subscribe alloc_mem failed! msg:{}.", msg);
                    return;
                }
                reqBuf.put(requestParams);
                msg.get_message_body().add_bytes_ex(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_REQUEST_PARAM, reqBuf);
            }
            clientProxyPtr.subscribe(topic, msg, clientMsgListerner);
        } catch (Exception e) {
            logger.error("Subscribe unexpected...topic({})!", topic);
        }
        logger.debug("Subscribe complete, topic({}).", topic);
    }

    public void unsubscribe(String topic, boolean subscribeOnly) {
        if (null != consumerInfo) {
            consumerInfo.subsConsumer = null;
        }

        //unsubscribe to lava box
        try {
            i_message msg = lava_box.create_message((short) lava_boxConstants.MSG_TYPE_LAVA_UNSUB);
            msg.set_topic(topic);
            if (subscribeOnly) {
                //0: subscribe 1: snapshot
                msg.get_message_body().add_int(LavaMessageEnum.LAVA_MSG_SUBS_TYPE, 0);
            } else {
                //0: subscribe 1: snapshot
                msg.get_message_body().add_int(LavaMessageEnum.LAVA_MSG_SUBS_TYPE, 1);
            }
            clientProxyPtr.unsubscribe(topic, msg, clientMsgListerner);
        } catch (Exception e) {
            logger.error("Unsubscribe unexpected...topic({})!", topic);
        }
        logger.debug("Unsubscribe complete, topic({}).", topic);
    }

    public void requestAsync(String queue, byte[] requestParams, Consumer<byte[]> bytesReplyConsumer) {
        if (0 == cookieAsyncRequest) {
            long[] cookie = { 0 };
            clientProxyPtr.advise(clientMsgListerner, cookie);
            cookieAsyncRequest = cookie[0];
        }

        i_message req_msg = lava_box.create_message((short) lava_boxConstants.MSG_TYPE_LAVA_REQ);
        req_msg.reference();
        req_msg.set_topic(queue);
        ByteBuffer inBuf = req_msg.get_message_body().alloc_mem(requestParams.length);
        if (null == inBuf) {
            logger.error("requestAsync: alloc_mem failed! queue:{}.", queue);
            return;
        }
        inBuf.put(requestParams);
        req_msg.get_message_body().add_bytes_ex(LavaMessageEnum.LAVA_MSG_REQUEST_BYTE, inBuf);
        clientProxyPtr.request_asyn(req_msg, cookieAsyncRequest);
        req_msg.dereference();
        asyncRequestConsumer = bytesReplyConsumer;
    }

    public byte[] requestSync(String queue, byte[] requestParams, int timeoutInMs) {
        try {
            i_message req_msg = lava_box.create_message((short) lava_boxConstants.MSG_TYPE_LAVA_REQ);
            req_msg.set_topic(queue);
            ByteBuffer inBuf = req_msg.get_message_body().alloc_mem(requestParams.length);
            if (null == inBuf) {
                logger.error("requestSync: alloc_mem failed! queue:{}.", queue);
                return null;
            }
            inBuf.put(requestParams);
            req_msg.get_message_body().add_bytes_ex(LavaMessageEnum.LAVA_MSG_REQUEST_BYTE, inBuf);
            i_message rsp_msg = new i_message();
            boolean ret = clientProxyPtr.request(req_msg, rsp_msg, timeoutInMs);
            if (ret) {
                ByteBuffer outBuf = rsp_msg.get_message_body().get_bytes(LavaMessageEnum.LAVA_MSG_REPLY_BYTE);
                if (null == outBuf) {
                    logger.error("requestSync: get_bytes failed!");
                    return null;
                }
                int bufLen = outBuf.limit() - outBuf.position();
                byte[] rspData = new byte[bufLen];
                outBuf.get(rspData);
                rsp_msg.dereference();
                return rspData;
            } else {
                logger.error("requestSync failed! timeout.");
                return null;
            }
        } catch (Exception e) {
            logger.error("requestSync unexpected...queue({}), reqParams({}), timeOut({})!",
                    queue, requestParams, timeoutInMs);
        }
        return null;
    }

    public void setEventConsumer(Consumer<Pair<Integer, String>> clientEventConsumer) {
        cliEvnConsumer = clientEventConsumer;
    }

    public void releaseLavaClient() {
        if (null != clientProxyPtr) {
            clientProxyPtr.cancel_event(clientEventListener);
        }
        if (null != clientEventListener) {
            clientEventListener.release();
        }
        if (0 != cookieAsyncRequest) {
            clientProxyPtr.unadvise(cookieAsyncRequest);
        }
        if (null != clientMsgListerner) {
            clientMsgListerner.release();
        }
        clientProxyPtr = null;
    }
}
