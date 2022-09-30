package sumscope.lava.mq;

import org.apache.commons.lang3.tuple.Pair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.nio.ByteBuffer;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.CopyOnWriteArraySet;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;
import java.util.function.UnaryOperator;
import sumscope.lava.*;

public class LavaServer {
    private class lavaServerEventListener extends i_event_listener {
        private AtomicInteger count = new AtomicInteger(0);

        public lavaServerEventListener() {
            super();
            add_ref();
        }

        /*---------------------------interfaces implementation--------------------------------------*/
        @Override
        public void on_event(int event_code, SWIGTYPE_p_void event_data) {
            String retMsg = new String();
            if (event_cluster_register_failed == event_code) {
                retMsg = "Cluster register failed!";
            } else if (event_cluster_node_on == event_code) {
                retMsg = "Cluster node on.";
            } else if (event_cluster_node_off == event_code) {
                retMsg = "Cluster node off.";
            } else {
                logger.info("Lava server received unrecognized event. EventCode: {}.", event_code);
                return;
            }
            if (null != mqServerEventListener) {
                mqServerEventListener.accept(Pair.of(event_code, retMsg));
                logger.info("Notify lava mq server listener. EventCode:{}, EventMsg:{}.", event_code, retMsg);
            } else {
                logger.warn("Lava mq server event listener not set, can not notify! EventCode:{}, EventMsg:{}.",
                        event_code, retMsg);
            }
        }

        @Override
        public void on_exception(int error_code, String what) {
            logger.error("Lava server event exception: err_code={}, what={}.", error_code, what);
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

    private class lavaServerListener extends i_server_listener {
        private AsyncThreadGroup<i_message> asyncThreadGroup = null;
        private AsyncThreadGroup<Object[]> postImageThreadGroup = null;
        private AtomicInteger count = new AtomicInteger(0);

        public lavaServerListener() {
            super();
            add_ref();
            asyncThreadGroup = new AsyncThreadGroup<i_message>(lavaConfig.getAsyncThreadCnt()) {
                protected void process(i_message data) {
                    lavaServerListener.sendReply(data);
                }
            };
            logger.info("Tread pool for async request create complete, thread count:{}.", lavaConfig.getAsyncThreadCnt());
            postImageThreadGroup = new AsyncThreadGroup<Object[]>(lavaConfig.getPostImageThreadCnt()) {
                protected void process(Object[] data) {
                    handleImagePush((String) data[0], (long) data[1], (byte[]) data[2]);
                }
            };
            logger.info("Tread pool for handling snapshot create complete, thread count:{}.", lavaConfig.getPostImageThreadCnt());
        }

        private void sendReply(i_message msg) {
            String queue = msg.get_topic();
            UnaryOperator<Pair<String, byte[]>> topicHandler = null;
            if (reqTopicCacheMap.containsKey(queue)) {
                topicHandler = reqTopicCacheMap.get(queue);
            } else {
                topicHandler = getTopicHandler(queue);
                if (null == topicHandler) {
                    msg.dereference();
                    return;
                } else {
                    reqTopicCacheMap.put(queue, topicHandler);
                }
            }

            i_message_body msgBody = msg.get_message_body();
            ByteBuffer reqBuf = msgBody.get_bytes(LavaMessageEnum.LAVA_MSG_REQUEST_BYTE);
            int bufLen = reqBuf.limit() - reqBuf.position();
            byte[] byteReqParams = new byte[bufLen];
            reqBuf.get(byteReqParams);
            byte[] byteRspData = topicHandler.apply(Pair.of(queue, byteReqParams)).getRight();
            ByteBuffer rspBuf = msg.get_message_body().alloc_mem(byteRspData.length);
            if (null == rspBuf) {
                logger.error("sendReply alloc_mem failed! msg:{}.", msg);
                msg.dereference();
                return;
            }
            rspBuf.put(byteRspData);
            msg.get_message_body().add_bytes_ex(LavaMessageEnum.LAVA_MSG_REPLY_BYTE, rspBuf);
            if (lava_boxConstants.MSG_TYPE_LAVA_REQ == msg.get_msg_type()) {
                msg.set_msg_type((short) lava_boxConstants.MSG_TYPE_LAVA_REPLY);
            } else {
                msg.set_msg_type((short) lava_boxConstants.MSG_TYPE_DATA_REPLY);
            }
            lavaServerPtr.reply_message(msg);
            msg.dereference();
        }

        private void handleImagePush(String topic, long connid, byte[] imageRequestParam) {
            // match the wildTopic and find the mq image server handler
            String wildTopic;
            UnaryOperator<Pair<String, byte[]>> svrSubImageHandler = null;
            LavaMq svrSubImageObj = null;
            int objCnt = 0;
            Set<LavaMq> mqImageSvrObjs = lavaMqImageServerObjsMap.keySet();
            for (LavaMq obj : mqImageSvrObjs) {
                wildTopic = lavaMqImageServerObjsMap.get(obj);
                // match topics
                if (TopicMatchBase.compareTopic(wildTopic, topic) >= 0) {
                    svrSubImageObj = (LavaMq)obj;
                    svrSubImageHandler = svrSubImageObj.getLavaMqServerSubImageHandler();
                    objCnt++;
                }
            }
            if (objCnt > 1) {
                logger.warn("handleImagePush: Image subscribe matched multi server objects! Object count:{}.", objCnt);
                logger.warn("handleImagePush: Other server objects will be covered with the last.");
            }
            if ((0 == objCnt) || (null == svrSubImageHandler) || (null == svrSubImageObj)) {
                logger.error("handleImagePush: Not found the matched image server object!");
                return;
            }

            // update pushCacheImageTopicsMap
            Long cidL = new Long(connid);
            CopyOnWriteArraySet<Long> imageCids = null;
            if (pushCacheImageTopicsMap.containsKey(topic)) {
                imageCids = pushCacheImageTopicsMap.get(topic);
                imageCids.add(cidL);
                pushCacheImageTopicsMap.replace(topic, imageCids);
            } else {
                imageCids = new CopyOnWriteArraySet<Long>();
                imageCids.add(cidL);
                pushCacheImageTopicsMap.put(topic, imageCids);
            }

            // acquire the snapshot data and push to client first
            byte[] imageData = svrSubImageHandler.apply(Pair.of(topic, imageRequestParam)).getRight();
            boolean ret = pushSnapshot(topic, connid, imageData);
            if (!ret) {
                // release cache data
                handleCacheData(topic, connid, false);

                String retMsg = "handleImagePush: Push snapshot failed! Release cache data!";
                logger.error(retMsg);
                // notify lava mq server event listener (image sub)
                notifySubResult(LavaMqEventCodeEnum.LAVA_CLIENT_SUBS_IMAGE_FAILED, retMsg);
                return;
            }

            // when push snapshot completed, we send the cache data
            handleCacheData(topic, connid, true);

            // update allSubWildTopicsMap
            Integer cidRef = 1;
            ConcurrentHashMap<Long, Integer> connidsMap = null;
            if (allSubWildTopicsMap.containsKey(topic)) {
                connidsMap = allSubWildTopicsMap.get(topic);
                if (connidsMap.containsKey(cidL)) {
                    cidRef = connidsMap.get(cidL);
                    cidRef++;
                    connidsMap.replace(cidL, cidRef);
                } else {
                    connidsMap.put(cidL, cidRef);
                }
                allSubWildTopicsMap.replace(topic, connidsMap);
            } else {
                connidsMap = new ConcurrentHashMap<>();
                connidsMap.put(cidL, cidRef);
                allSubWildTopicsMap.put(topic, connidsMap);
            }

            // update pushAllTopicsMap
            CopyOnWriteArraySet<Long> pushCids = null;
            if (pushAllTopicsMap.containsKey(topic)) {
                pushCids = pushAllTopicsMap.get(topic);
                pushCids.add(cidL);
                pushAllTopicsMap.replace(topic, pushCids);
            } else {
                pushCids = new CopyOnWriteArraySet<Long>();
                pushCids.add(cidL);
                pushAllTopicsMap.put(topic, pushCids);
                recordPushTopicCache(pushAllTopicsMap.size());
            }

            // notify lava mq server event listener (image sub)
            notifySubResult(LavaMqEventCodeEnum.LAVA_CLIENT_SUBS_IMAGE_SUCCESS, topic);
        }

        private boolean pushSnapshot(String topic, long connid, byte[] bytes) {
            i_message msg = lava_box.create_message((short) lava_boxConstants.MSG_TYPE_LAVA_PUSH);
            msg.set_topic(topic);
            ByteBuffer pushBuf = msg.get_message_body().alloc_mem(bytes.length);
            if (null == pushBuf) {
                logger.error("pushSnapshot: alloc_mem failed! topic:{}", topic);
                return false;
            }
            pushBuf.put(bytes);
            msg.get_message_body().add_bytes_ex(LavaMessageEnum.LAVA_MSG_SUBS_RESPONSE_DATA, pushBuf);
            msg.get_message_body().add_string(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_MSGID, imageSubFilter);
            msg.get_message_body().add_long(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_CACHE_MSGID, imageSubCacheFilter);
            msg.set_conn_id(connid);
            lavaServerPtr.publish_message(msg, topic);
            return true;
        }

        // handleType=true (send cache data), handleType=false (release cache data)
        private void handleCacheData(String topic, long connid, boolean handleType) {
            Long connidL = new Long(connid);
            if (!pushCacheImageMsgsMap.containsKey(connidL)) {
                return;
            }
            CopyOnWriteArrayList<i_message> lstMsg = pushCacheImageMsgsMap.remove(connidL);

            // update pushCacheImageTopicsMap
            Long cidL = new Long(connid);
            CopyOnWriteArraySet<Long> imageCids = null;
            if (pushCacheImageTopicsMap.containsKey(topic)) {
                imageCids = pushCacheImageTopicsMap.get(topic);
                imageCids.remove(cidL);
                if (imageCids.isEmpty()) {
                    pushCacheImageTopicsMap.remove(topic);
                } else {
                    pushCacheImageTopicsMap.replace(topic, imageCids);
                }
            }

            if (handleType) {
                for (i_message cacheMsg : lstMsg) {
                    cacheMsg.set_conn_id(connid);
                    lavaServerPtr.publish_message(cacheMsg, topic);
                }
            }
        }

        @Override
        public i_server_listener.auth_result on_authentication(String username, String password) {
            return i_server_listener.auth_result.identity_ok;
        }

        @Override
        public void on_request(i_message msg) {
            msg.reference();
            asyncThreadGroup.add(msg);
        }

        @Override
        public void on_subscribe(String wildTopic, i_message msg) {
            // get connection id
            long connid = msg.get_conn_id();
            Long connidL = new Long(connid);

            // get subscribe type and handle it
            long subType = msg.get_message_body().get_int(LavaMessageEnum.LAVA_MSG_SUBS_TYPE);
            // 0: subscribe 1: snapshot
            if (1 == subType) {
                // 0: needn't push snapshot，other: push snapshot
                short replyFlag = msg.get_flags();
                if (0 != (replyFlag & 0x10)) {
                    ByteBuffer imageReqBuf = msg.get_message_body().get_bytes(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_REQUEST_PARAM);
                    int bufLen = imageReqBuf.limit() - imageReqBuf.position();
                    byte[] byteBuf = new byte[bufLen];
                    imageReqBuf.get(byteBuf);
                    // if it's snapshot mode, the wildTopic must be an exact topic.
                    postImageThreadGroup.add(new Object[]{wildTopic, connid, byteBuf});
                }
            } else {
                /* handle common subscribe */
                // update allSubWildTopicsMap
                Integer cidRef = 1;
                ConcurrentHashMap<Long, Integer> connidsMap = null;
                if (allSubWildTopicsMap.containsKey(wildTopic)) {
                    connidsMap = allSubWildTopicsMap.get(wildTopic);
                    if (connidsMap.containsKey(connidL)) {
                        cidRef = connidsMap.get(connidL);
                        cidRef++;
                        connidsMap.replace(connidL, cidRef);
                    } else {
                        connidsMap.put(connidL, cidRef);
                    }
                    allSubWildTopicsMap.replace(wildTopic, connidsMap);
                } else {
                    connidsMap = new ConcurrentHashMap<>();
                    connidsMap.put(connidL, cidRef);
                    allSubWildTopicsMap.put(wildTopic, connidsMap);
                }

                // update pushAllTopicsMap
                if (!pushAllTopicsMap.isEmpty()) {
                    Set<String> allSubTopics = pushAllTopicsMap.keySet();
                    for (String topic : allSubTopics) {
                        // match topics for pushAllTopicsMap
                        if (TopicMatchBase.compareTopic(wildTopic, topic) >= 0) {
                            CopyOnWriteArraySet<Long> cids = pushAllTopicsMap.get(topic);
                            cids.add(connidL);
                            pushAllTopicsMap.replace(topic, cids);
                        }
                    }
                }

                // reply subscribe result to lava mq server event listener(common sub)
                notifySubResult(LavaMqEventCodeEnum.LAVA_CLIENT_SUBS_SUCCESS, wildTopic);
            }
        }

        @Override
        public void on_unsubscribe(String wildTopic, i_message msg) {
            long connid = msg.get_conn_id();
            Long connidL = new Long(connid);
            boolean unsubResult = false;
            String retMsg = "";

            // update allSubWildTopicsMap
            ConcurrentHashMap<Long, Integer> allCidsRef = null;
            Integer cidRef = 0;
            if (allSubWildTopicsMap.containsKey(wildTopic)) {
                allCidsRef = allSubWildTopicsMap.get(wildTopic);
                if (allCidsRef.containsKey(connidL)) {
                    cidRef = allCidsRef.get(connidL);
                    cidRef--;
                    if (0 == cidRef) {
                        allCidsRef.remove(connidL);
                        if (allCidsRef.isEmpty()) {
                            allSubWildTopicsMap.remove(wildTopic);
                            // while cid reference down to zero, we can clear connIds from pushAllTopicsMap
                            if (pushAllTopicsMap.containsKey(wildTopic)) {
                                // if the wildTopic is an exact topic, remove directly
                                pushAllTopicsMap.remove(wildTopic);
                                recordPushTopicCache(pushAllTopicsMap.size());
                            } else {
                                // if the wildTopic is real wild, match it and release the cid
                                Set<String> allPushTopics = pushAllTopicsMap.keySet();
                                CopyOnWriteArraySet<Long> topicCids = null;
                                for (String pushTopic : allPushTopics) {
                                    if (TopicMatchBase.compareTopic(wildTopic, pushTopic) >= 0) {
                                        topicCids = pushAllTopicsMap.get(pushTopic);
                                        topicCids.remove(connidL);
                                        if (topicCids.isEmpty()) {
                                            pushAllTopicsMap.remove(pushTopic);
                                            recordPushTopicCache(pushAllTopicsMap.size());
                                        } else {
                                            pushAllTopicsMap.replace(pushTopic, topicCids);
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        allCidsRef.replace(connidL, cidRef);
                    }
                    allSubWildTopicsMap.replace(wildTopic, allCidsRef);
                    unsubResult = true;
                } else {
                    retMsg = "Cannot find lava mq connect id! Maybe it has not subscribed yet. connId:" + connidL.toString();
                }
            } else {
                retMsg = "Cannot find the unsubscribe topic! Maybe it has not subscribed yet. wildTopic:" + wildTopic;
            }

            // reply unsubscribe result to lava mq server event listener
            long subType = msg.get_message_body().get_int(LavaMessageEnum.LAVA_MSG_SUBS_TYPE);
            // 0: subscribe 1: snapshot
            if (0 == subType) {
                if (unsubResult) {
                    notifySubResult(LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_SUCCESS, wildTopic);
                } else {
                    notifySubResult(LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_FAILED, retMsg);
                }
            } else {
                if (unsubResult) {
                    notifySubResult(LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_IMAGE_SUCCESS, wildTopic);
                } else {
                    notifySubResult(LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_IMAGE_FAILED, retMsg);
                }
            }
        }

        @Override
        // when the client is not exist,
        // server will receive the event and remove the client's connect id,
        // and will not push data to that client again.
        public void on_disconnected(long conn_id) {
            Long connidL = new Long(conn_id);
            logger.debug("on_disconnected start... connId: {}.", connidL);

            // update pushCacheImageTopicsMap
            CopyOnWriteArraySet<Long> cacheCids = null;
            Set<String> cacheTopics = null;
            cacheTopics = pushCacheImageTopicsMap.keySet();
            for (String cacheTopic : cacheTopics) {
                cacheCids = pushCacheImageTopicsMap.get(cacheTopic);
                cacheCids.remove(connidL);
                if (cacheCids.isEmpty()) {
                    pushCacheImageTopicsMap.remove(cacheTopic);
                } else {
                    pushCacheImageTopicsMap.replace(cacheTopic, cacheCids);
                }
            }
            logger.debug("on_disconnected: Handle pushCacheImageTopicsMap complete.");

            // update pushCacheImageMsgsMap
            pushCacheImageMsgsMap.remove(connidL);
            logger.debug("on_disconnected: Handle pushCacheImageMsgsMap complete.");

            // update allSubWildTopicsMap
            ConcurrentHashMap<Long, Integer> allSubCids = null;
            Set<String> allSubWildTopics = null;
            allSubWildTopics = allSubWildTopicsMap.keySet();
            for (String allSubWildTopic : allSubWildTopics) {
                allSubCids = allSubWildTopicsMap.get(allSubWildTopic);
                allSubCids.remove(connidL);
                if (allSubCids.isEmpty()) {
                    allSubWildTopicsMap.remove(allSubWildTopic);
                } else {
                    allSubWildTopicsMap.replace(allSubWildTopic, allSubCids);
                }
            }
            logger.debug("on_disconnected: Handle allSubWildTopicsMap complete.");

            // update pushAllTopicsMap
            CopyOnWriteArraySet<Long> allPushCids = null;
            Set<String> allPushTopics = null;
            allPushTopics = pushAllTopicsMap.keySet();
            for (String allPushTopic : allPushTopics) {
                allPushCids = pushAllTopicsMap.get(allPushTopic);
                allPushCids.remove(connidL);
                if (allPushCids.isEmpty()) {
                    pushAllTopicsMap.remove(allPushTopic);
                    recordPushTopicCache(pushAllTopicsMap.size());
                } else {
                    pushAllTopicsMap.replace(allPushTopic, allPushCids);
                }
            }
            logger.debug("on_disconnected: Handle pushAllTopicsMap complete.");
            logger.debug("on_disconnected complete. connId: {}.", connidL);
            logger.warn("on_disconnected happened! Lava client connId: [{}].", connidL);
        }

        @Override
        public int add_ref() {
            return count.incrementAndGet();
        }

        @Override
        public int release() {
            int ret = count.decrementAndGet();
            if (ret <= 0) {
                if (null != asyncThreadGroup) {
                    asyncThreadGroup.dispose();
                    asyncThreadGroup = null;
                }
                if (null != postImageThreadGroup) {
                    postImageThreadGroup.dispose();
                    postImageThreadGroup = null;
                }
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

    private static String imageSubFilter = "SUBS_IMAGE_FILTER";
    private static long imageSubCacheFilter = 99L;
    private static int maxCacheMsgCnt = 10000;
    private static int warnPushTopicCacheBaseCnt = 10 * 10000;
    private static int warnPushTopicCacheNextCnt = warnPushTopicCacheBaseCnt;
    private static int warnPushTopicCacheBeforeCnt = 0;
    private Logger logger = LoggerFactory.getLogger(this.getClass());
    private String url = "";
    private lavaServerListener lavaServerListener = null;
    private lavaServerEventListener lavaServerEventListener = null;
    private stub_auto_ptr lavaServerPtr = null;
    private LavaConfig lavaConfig = null;
    private ConcurrentHashMap<String, UnaryOperator<Pair<String, byte[]>>> requestHandlerMap = null;
    private ConcurrentHashMap<String, UnaryOperator<Pair<String, byte[]>>> reqTopicCacheMap = null;
    private Consumer<Pair<Integer, String>> mqServerEventListener = null;
    private ConcurrentHashMap<LavaMq, String> lavaMqImageServerObjsMap = null;
    private ConcurrentHashMap<String, CopyOnWriteArraySet<Long>> pushAllTopicsMap = null;
    private ConcurrentHashMap<String, CopyOnWriteArraySet<Long>> pushCacheImageTopicsMap = null;
    private ConcurrentHashMap<Long, CopyOnWriteArrayList<i_message>> pushCacheImageMsgsMap = null;
    private ConcurrentHashMap<String, ConcurrentHashMap<Long, Integer>> allSubWildTopicsMap = null;

    public LavaServer(String url, LavaConfig conf) {
        this.url = url;
        lavaConfig = conf;
        lavaServerEventListener = new LavaServer.lavaServerEventListener();
        requestHandlerMap = new ConcurrentHashMap<>();
        reqTopicCacheMap = new ConcurrentHashMap<>();
        lavaMqImageServerObjsMap = new ConcurrentHashMap<>();
        pushAllTopicsMap = new ConcurrentHashMap<>();
        pushCacheImageTopicsMap = new ConcurrentHashMap<>();
        pushCacheImageMsgsMap = new ConcurrentHashMap<>();
        allSubWildTopicsMap = new ConcurrentHashMap<>();
        logger.info("Lava server create complete.");
    }

    public void registerRequestHandler(String wildTopic, UnaryOperator<Pair<String, byte[]>> opt) {
        if (!requestHandlerMap.containsKey(wildTopic)) {
            requestHandlerMap.put(wildTopic, opt);
        } else {
            requestHandlerMap.replace(wildTopic, opt);
        }
    }

    public void removeRequestHandler(String wildTopic) {
        requestHandlerMap.remove(wildTopic);
    }

    public void regLavaMqImageServer(LavaMq lavamqImageSrvObj, String wildTopic) {
        if (lavaMqImageServerObjsMap.containsKey(lavamqImageSrvObj)) {
            lavaMqImageServerObjsMap.replace(lavamqImageSrvObj, wildTopic);
        } else {
            lavaMqImageServerObjsMap.put(lavamqImageSrvObj, wildTopic);
        }
    }

    public void unregLavaMqImageServer(LavaMq lavamqImageSrvObj) {
        lavaMqImageServerObjsMap.remove(lavamqImageSrvObj);
    }

    public void setEventConsumer(Consumer<Pair<Integer, String>> serverEventConsumer) {
        mqServerEventListener = serverEventConsumer;
    }

    public void notifySubResult(int retCode, String retTopic) {
        String debugMsg;
        String warnMsg;
        switch (retCode) {
            case LavaMqEventCodeEnum.LAVA_CLIENT_SUBS_SUCCESS:
                debugMsg = "Lava mq client subscribe success. retCode:{}, retTopic:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify subscribe result! retCode:{}, retTopic:{}.";
                break;
            case LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_SUCCESS:
                debugMsg = "Lava mq client unsubscribe success. retCode:{}, retTopic:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify unsubscribe result! retCode:{}, retTopic:{}.";
                break;
            case LavaMqEventCodeEnum.LAVA_CLIENT_SUBS_IMAGE_SUCCESS:
                debugMsg = "Lava mq client image subscribe success. retCode:{}, retTopic:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify image subscribe result! retCode:{}, retTopic:{}.";
                break;
            case LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_IMAGE_SUCCESS:
                debugMsg = "Lava mq client image unsubscribe success. retCode:{}, retTopic:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify image unsubscribe result! retCode:{}, retTopic:{}.";
                break;
            case LavaMqEventCodeEnum.LAVA_CLIENT_SUBS_FAILED:
                debugMsg = "Lava mq client subscribe failed! retCode:{}, retMessage:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify subscribe result! retCode:{}, retMessage:{}.";
                break;
            case LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_FAILED:
                debugMsg = "Lava mq client unsubscribe failed! retCode:{}, retMessage:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify unsubscribe result! retCode:{}, retMessage:{}.";
                break;
            case LavaMqEventCodeEnum.LAVA_CLIENT_SUBS_IMAGE_FAILED:
                debugMsg = "Lava mq client image subscribe failed! retCode:{}, retMessage:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify image subscribe result! retCode:{}, retMessage:{}.";
                break;
            case LavaMqEventCodeEnum.LAVA_CLIENT_UNSUBS_IMAGE_FAILED:
                debugMsg = "Lava mq client image unsubscribe failed! retCode:{}, retMessage:{}.";
                warnMsg = "Lava mq server event listener not set, can not notify image unsubscribe result! retCode:{}, retMessage:{}.";
                break;
            default:
                logger.warn("Unrecognized retCode! retCode:{}, retMessage:{}.", retCode, retTopic);
                return;
        }

        // reply subscribe result to lava mq server event listener
        if (null != mqServerEventListener) {
            logger.debug(debugMsg, retCode, retTopic);
            mqServerEventListener.accept(Pair.of(retCode, retTopic));
        } else {
            logger.warn(warnMsg, retCode, retTopic);
        }
    }

    public void init() {
        // create lava server listener
        if (null == lavaServerListener) {
            lavaServerListener = new lavaServerListener();
        }
    }

    public void publishBytesMsg(String topic, String msgid, byte[] bytes) {
        // create message
        i_message msg = lava_box.create_message((short) lava_boxConstants.MSG_TYPE_LAVA_PUSH);
        msg.set_topic(topic);
        ByteBuffer pushBuf = msg.get_message_body().alloc_mem(bytes.length);
        if (null == pushBuf) {
            logger.error("publishBytesMsg: alloc_mem failed! topic:{}", topic);
            return;
        }
        pushBuf.put(bytes);
        msg.get_message_body().add_bytes_ex(LavaMessageEnum.LAVA_MSG_SUBS_RESPONSE_DATA, pushBuf);
        msg.get_message_body().add_string(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_MSGID, msgid);
        msg.reference();

        /* handle push for image subscribe */
        CopyOnWriteArraySet<Long> imageCids = null;
        // need cache messages for image subscribe
        if (pushCacheImageTopicsMap.containsKey(topic)) {
            imageCids = pushCacheImageTopicsMap.get(topic);
            for (Long connidL : imageCids) {
                CopyOnWriteArrayList<i_message> lstMsg = null;
                i_message cloneMsg = null;
                if (pushCacheImageMsgsMap.containsKey(connidL)) {
                    lstMsg = pushCacheImageMsgsMap.get(connidL);
                    int cacheMsgCnt = lstMsg.size();
                    if (cacheMsgCnt > maxCacheMsgCnt) {
                        lstMsg.clear();
                        logger.error("publishBytesMsg: Cache too many messages! Discard all cache for connID:[{}], topic:[{}], dropCount:[{}].",
                                connidL, topic, cacheMsgCnt);
                    }
                    cloneMsg = msg.clone();
                    cloneMsg.get_message_body().add_long(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_CACHE_MSGID, imageSubCacheFilter);
                    lstMsg.add(cloneMsg);
                    pushCacheImageMsgsMap.replace(connidL, lstMsg);
                } else {
                    lstMsg = new CopyOnWriteArrayList<>();
                    cloneMsg = msg.clone();
                    cloneMsg.get_message_body().add_long(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_CACHE_MSGID, imageSubCacheFilter);
                    lstMsg.add(cloneMsg);
                    pushCacheImageMsgsMap.put(connidL, lstMsg);
                }
            }
        }

        /* handle push for common subscribe */
        CopyOnWriteArraySet<Long> allSubCids = null;
        if (pushAllTopicsMap.containsKey(topic)) {
            allSubCids = pushAllTopicsMap.get(topic);
        } else {
            allSubCids = new CopyOnWriteArraySet<Long>();
            // need match the wild topics
            Set<String> wildTopics =  allSubWildTopicsMap.keySet();
            for (String wildTopic : wildTopics) {
                if (TopicMatchBase.compareTopic(wildTopic, topic) >= 0) {
                    ConcurrentHashMap<Long, Integer> cidsMap = allSubWildTopicsMap.get(wildTopic);
                    Set<Long> cids = cidsMap.keySet();
                    allSubCids.addAll(cids);
                }
            }
            // update pushAllTopicsMap
            pushAllTopicsMap.put(topic, allSubCids);
            recordPushTopicCache(pushAllTopicsMap.size());
        }

        // push messages for all subscribe
        i_message cloneMsg = null;
        for (Long connidL : allSubCids) {
            cloneMsg = msg.clone();
            cloneMsg.get_message_body().add_long(LavaMessageEnum.LAVA_MSG_SUBS_IMAGE_CACHE_MSGID, 0);
            cloneMsg.set_conn_id(connidL.longValue());
            lavaServerPtr.publish_message(cloneMsg, topic);
        }
        msg.dereference();
    }

    public boolean start() {
        try {
            if (null == lavaServerPtr) {
                String host = lavaConfig.getHost();
                String port = lavaConfig.getPort();
                String serverAddr = host + ":" + port;
                logger.info("Lava server address: {}. ", serverAddr);
                if (lavaConfig.isUseCluster()) {
                    lava_service_type lavaSrvType;
                    String svrType = "";
                    switch (lavaConfig.getServiceType()) {
                        case 1:
                            lavaSrvType = lava_service_type.lst_all;
                            svrType = "lst_all";
                            break;
                        case 2:
                            lavaSrvType = lava_service_type.lst_standby;
                            svrType = "lst_standby";
                            break;
                        default:
                            lavaSrvType = lava_service_type.lst_unknow;
                            svrType = "lst_unknow";
                    }
                    logger.info("Lava server use cluster. Service type: {}. ", svrType);
                    lavaServerPtr = lava_box.create_stub_for_java(this.url, lavaServerListener, lavaServerEventListener,
                            serverAddr, comm_protocol.tcp_comm, lavaConfig.getNodeInfo(), this.url, lavaSrvType);
                    if (null == lavaServerPtr) {
                        logger.error("Lava server create stub failed! (cluster mode)");
                        return false;
                    }
                    logger.info("Lava server create stub success. (cluster mode)");
                } else {
                    lavaServerPtr = lava_box.create_stub_for_java(this.url, lavaServerListener,
                            serverAddr, comm_protocol.tcp_comm);
                    if (null == lavaServerPtr) {
                        logger.error("Lava server create stub failed! (direct mode)");
                        return false;
                    }
                    logger.info("Lava server create stub success.(direct mode)");
                }
            }
            return true;
        } catch (Exception e) {
            logger.error("Lava server start failed! err={}", e);
        }
        return false;
    }

    public void releaseLavaServer() {
        if (null != lavaServerListener) {
            lavaServerListener.release();
        }
        if (null != lavaServerEventListener) {
            lavaServerEventListener.release();
        }
    }

    private UnaryOperator<Pair<String, byte[]>> getTopicHandler(String targetTopic) {
        if (requestHandlerMap.containsKey(targetTopic)) {
            // 完全匹配
            return requestHandlerMap.get(targetTopic);
        }
        Set<String> sourceTopics = requestHandlerMap.keySet();
        // 非"**" 匹配
        for (String sourceTopic : sourceTopics) {
            if (!sourceTopic.contains("**")) {
                if (TopicMatchBase.compareTopic(sourceTopic, targetTopic) >= 0) {
                    return requestHandlerMap.get(sourceTopic);
                }
            }
        }
        // "**" 匹配
        for (String sourceTopic : sourceTopics) {
            if (TopicMatchBase.compareTopic(sourceTopic, targetTopic) >= 0) {
                return requestHandlerMap.get(sourceTopic);
            }
        }
        return null;
    }

    private void recordPushTopicCache(int size) {
        if (size >= warnPushTopicCacheNextCnt) {
            logger.warn("pushAllTopicsMap element count increment to {}!", size);
            warnPushTopicCacheBeforeCnt = size;
            warnPushTopicCacheNextCnt = size + warnPushTopicCacheBaseCnt;
        } else {
            if (size < warnPushTopicCacheBeforeCnt) {
                warnPushTopicCacheNextCnt = size;
                warnPushTopicCacheBeforeCnt = size - warnPushTopicCacheBaseCnt;
                warnPushTopicCacheBeforeCnt = Math.max(warnPushTopicCacheBeforeCnt, 0);
                logger.info("pushAllTopicsMap element count down to {}.", size);
            }
        }
    }
}
