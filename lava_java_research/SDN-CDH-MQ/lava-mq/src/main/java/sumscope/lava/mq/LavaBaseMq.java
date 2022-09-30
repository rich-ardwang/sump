package sumscope.lava.mq;

import org.apache.commons.lang3.tuple.Pair;
import java.util.function.Consumer;
import java.util.function.UnaryOperator;

public interface LavaBaseMq {
    void initConfig(LavaConfig conf);
    void initClientSubscribeBytes(String url, String wildTopic, Consumer<Pair<String, byte[]>> subscribeConsumer);
    void initClientSubscribeImageBytes(String url, String topic, byte[] requestParams, Consumer<Pair<String, byte[]>> subscribeConsumer);
    void initClientRequest(String url);
    void initServerPublish(String url, String wildTopic, UnaryOperator<Pair<String, byte[]>> imageHandler);
    void initServerResponseBytes(String url, String queue, UnaryOperator<Pair<String, byte[]>> bytesResponseHandler);

    // client (success login, dissconted)
    void setServerEventListener(Consumer<Pair<Integer, String>> serverEventConsumer);
    //cluster(node on, node off, cluster failed)
    void setClientEventListener(Consumer<Pair<Integer, String>> clientEventConsumer);

    void start();
    boolean isRunning();
    void stop();
    void publishBytes(String msgid, String topic, byte[] bytes);
    void requestBytesAsync(String queue, byte[] requestParams, Consumer<byte[]> bytesReplyConsumer);
    byte[] requestBytesSync(String queue, byte[] requestParams, int timeoutInMs);

    void releaseLavaMq();

    static LavaBaseMq createLavaMq() {
        return new LavaMq();
    }
}
