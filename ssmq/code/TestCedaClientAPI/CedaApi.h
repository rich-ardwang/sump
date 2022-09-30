#ifndef CEDA_CLIENT
#define CEDA_CLIENT
class ClientOne;
class CedaClient {
public:
	CedaClient(void);
	virtual ~CedaClient(void);
	
	//��Ӧ��Ϣ
	virtual void onMessage(const char* topic, const char *content, int len){};
	
	//��Ӧ�����¼�
	virtual void onEvent(int nevent){};

	//��¼AMQ��ʹ��ValidationServer��֤��¼
	void setLoginMessage(char *str);

	//ʹ���û����������¼
	void setUserPass(char *user, char *pass);
	
	//0:don't auto reconnect;1, auto reconnect(no resubscribe);2,auto reconnect(resubscribe), Ĭ��Ϊ1
	void setReconnectMode(int nMode);

	//��������Ϊ�������ͣ����������host������������˿ڣ������û��������û����룻
	//��������ProxySocks4��1; ProxySocks5��2�� ProxyHTTP��3�� ProxyIE��4
	void setProxyInfo(int proxyType, char *proxyHost, int proxyPort, char* proxyUser, char* proxyPass);

	//��������Ϊhost��port�����ӷ�ʽ��1��tcp��2��tcps
	void connect(char *host, int port, int protocol);

	//ֹͣ����
	void stop();

	//#�������ģ�ʹ�á�|���ָ�topic
	void subscribe(char *topics);

	//#�������ģ�ʹ�á�|���ָ�topic
	void subscribe(char *topics, char *svrID);

	//#�����˶���Ϣ��ʹ�á�|���ָ�topic
	void unSubscribe(char *topics);

	//#������Ϣ����������Ϊtopic,���ݣ�svrid���Ƿ�ѹ��
	void send(char *topic, char *content, int isGZip);

	//#request/reply��Ϣ����������Ϊtopic,���ݣ�svrid���Ƿ�ѹ�����ȴ�����ʱ��(s)
	char *request(char *topic, char *content, char *svrID, int isGZip, int deleys);
	
	//�첽request����������Ϊtopic,���ݣ�svrid���Ƿ�ѹ����ͨ��onMessage������Ϣ
	void asyncRequest(char *topic, char *content, char *svrID, int isGZip);

	ClientOne *m_clientOne;
};


class CedaProxy {
public:
	CedaProxy();
	~CedaProxy();

	//��־�ļ�����
	void setLogFile(char *fileName);
	
	//#��־����3��debug��0��info
	void setLogLevel(int level);
	
	//#��ӡ��־,������0:ERROR, 1:WARN,2:INFO,3:DEBUG
	void logString(int ntype, const char* str);
	
	//�����Ƿ����������̨��Ĭ�ϲ����
	void toConsole(bool bConsole);

	//���client
	void addClient(CedaClient *cb);
	void removeClient(CedaClient *cb);


};

#endif