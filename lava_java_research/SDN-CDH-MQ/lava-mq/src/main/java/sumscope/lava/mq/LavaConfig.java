package sumscope.lava.mq;

public class LavaConfig implements Cloneable {
    private boolean useCluster = true;
    private String host = new String("127.0.0.1");
    private String port = new String("10001");
    private String nodeInfo = new String("127.0.0.1:2181");
    private int serviceType = 1;
    private int asyncThreadCnt = 4;
    private int postImageThreadCnt = 4;
    private String userName = new String("");
    private String passWord = new String("");
    private boolean shareSession = true;
    private boolean autoConnect = true;
    private long tryTimes = 3;
    private boolean useProxy = false;
    private String proxyHost = new String("");
    private String proxyPort = new String("");
    private int proxyType = 1;
    private String proxyUsername = new String("");
    private String proxyPassword = new String("");
    private long clientIOThreads = 2;
    private long serverWorkerThreads = 4;

    public boolean isUseCluster() {
        return useCluster;
    }

    public void setUseCluster(boolean useCluster) {
        this.useCluster = useCluster;
    }

    public String getHost() {
        return host;
    }

    public void setHost(String host) {
        this.host = host;
    }

    public String getPort() {
        return port;
    }

    public void setPort(String port) {
        this.port = port;
    }

    public String getNodeInfo() {
        return nodeInfo;
    }

    public void setNodeInfo(String nodeInfo) {
        this.nodeInfo = nodeInfo;
    }

    public int getServiceType() {
        return serviceType;
    }

    public void setServiceType(int serviceType) {
        this.serviceType = serviceType;
    }

    public int getAsyncThreadCnt() {
        return asyncThreadCnt;
    }

    public void setAsyncThreadCnt(int asyncThreadCnt) {
        this.asyncThreadCnt = asyncThreadCnt;
    }

    public int getPostImageThreadCnt() {
        return postImageThreadCnt;
    }

    public void setPostImageThreadCnt(int postImageThreadCnt) {
        this.postImageThreadCnt = postImageThreadCnt;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getPassWord() {
        return passWord;
    }

    public void setPassWord(String passWord) {
        this.passWord = passWord;
    }

    public boolean isShareSession() {
        return shareSession;
    }

    public void setShareSession(boolean shareSession) {
        this.shareSession = shareSession;
    }

    public boolean isAutoConnect() {
        return autoConnect;
    }

    public void setAutoConnect(boolean autoConnect) {
        this.autoConnect = autoConnect;
    }

    public long getTryTimes() {
        return tryTimes;
    }

    public void setTryTimes(long tryTimes) {
        this.tryTimes = tryTimes;
    }

    public boolean isUseProxy() {
        return useProxy;
    }

    public void setUseProxy(boolean useProxy) {
        this.useProxy = useProxy;
    }

    public String getProxyHost() {
        return proxyHost;
    }

    public void setProxyHost(String proxyHost) {
        this.proxyHost = proxyHost;
    }

    public String getProxyPort() {
        return proxyPort;
    }

    public void setProxyPort(String proxyPort) {
        this.proxyPort = proxyPort;
    }

    public int getProxyType() {
        return proxyType;
    }

    public void setProxyType(int proxyType) {
        this.proxyType = proxyType;
    }

    public String getProxyUsername() {
        return proxyUsername;
    }

    public void setProxyUsername(String proxyUsername) {
        this.proxyUsername = proxyUsername;
    }

    public String getProxyPassword() {
        return proxyPassword;
    }

    public void setProxyPassword(String proxyPassword) {
        this.proxyPassword = proxyPassword;
    }

    public long getClientIOThreads() {
        return this.clientIOThreads;
    }

    public void setClientIOThreads(long clientIOThreads) {
        this.clientIOThreads = clientIOThreads;
    }

    public long getServerWorkerThreads() {
        return this.serverWorkerThreads;
    }

    public void setServerWorkerThreads(long serverWorkerThreads) {
        this.serverWorkerThreads = serverWorkerThreads;
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
