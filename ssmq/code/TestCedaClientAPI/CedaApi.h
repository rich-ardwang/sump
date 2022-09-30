#ifndef CEDA_CLIENT
#define CEDA_CLIENT
class ClientOne;
class CedaClient {
public:
	CedaClient(void);
	virtual ~CedaClient(void);
	
	//响应消息
	virtual void onMessage(const char* topic, const char *content, int len){};
	
	//响应连接事件
	virtual void onEvent(int nevent){};

	//登录AMQ，使用ValidationServer验证登录
	void setLoginMessage(char *str);

	//使用用户名，密码登录
	void setUserPass(char *user, char *pass);
	
	//0:don't auto reconnect;1, auto reconnect(no resubscribe);2,auto reconnect(resubscribe), 默认为1
	void setReconnectMode(int nMode);

	//参数依次为代理类型，代理服务器host，代理服务器端口，代理用户，代理用户密码；
	//代理类型ProxySocks4：1; ProxySocks5：2； ProxyHTTP：3； ProxyIE：4
	void setProxyInfo(int proxyType, char *proxyHost, int proxyPort, char* proxyUser, char* proxyPass);

	//参数依次为host，port；连接方式，1：tcp，2：tcps
	void connect(char *host, int port, int protocol);

	//停止重连
	void stop();

	//#批量订阅，使用‘|’分隔topic
	void subscribe(char *topics);

	//#批量订阅，使用‘|’分隔topic
	void subscribe(char *topics, char *svrID);

	//#批量退订消息，使用‘|’分隔topic
	void unSubscribe(char *topics);

	//#发送消息，参数依次为topic,内容，svrid，是否压缩
	void send(char *topic, char *content, int isGZip);

	//#request/reply消息，参数依次为topic,内容，svrid，是否压缩，等待返回时间(s)
	char *request(char *topic, char *content, char *svrID, int isGZip, int deleys);
	
	//异步request，参数依次为topic,内容，svrid，是否压缩，通过onMessage返回消息
	void asyncRequest(char *topic, char *content, char *svrID, int isGZip);

	ClientOne *m_clientOne;
};


class CedaProxy {
public:
	CedaProxy();
	~CedaProxy();

	//日志文件名称
	void setLogFile(char *fileName);
	
	//#日志级别，3：debug，0：info
	void setLogLevel(int level);
	
	//#打印日志,参数：0:ERROR, 1:WARN,2:INFO,3:DEBUG
	void logString(int ntype, const char* str);
	
	//设置是否输出到控制台，默认不输出
	void toConsole(bool bConsole);

	//添加client
	void addClient(CedaClient *cb);
	void removeClient(CedaClient *cb);


};

#endif