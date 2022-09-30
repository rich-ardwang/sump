#include "StdAfx.h"
#include "CedaATSMQMng.h"


#define  UNIQUE_EVENTNAME_RGCEDASUCESS "12528124-7BA3-44B1-9B62-CAC2A29C77BF"
#define  UNIQUE_ASYNC_SEND_SUCESS "F3067DB3-8735-4996-A972-085F5EE21FA1"


extern HMODULE AfxGetCurrentModule();

//internal use
CCedaATSMQMng* GetCedaATSMQMngIns()
{
    static CCedaATSMQMng sRet;
    return &sRet;
}

//external use
CedaATSMQ::ICedaATSMQMng* GetCedaATSMQMng()
{
    return GetCedaATSMQMngIns();
}

CCedaATSMQMng::CCedaATSMQMng(void)
: m_pMngCallback(NULL)
 ,m_bIsCedaInit(false)
 ,m_hRegCedaSucessEvent(NULL)
 ,m_hCedaSendRetEvent(NULL)
 ,m_sendBuf("")
 ,m_nUid(0)
{
    InitConfig();
    InitializeCriticalSection(&m_cs);
    //m_pLogger = new ss::logger("log\\CedaATSMQ");
    //m_pLogger->set_level_by_config("config\\logconfig.ini", "maincfg.cedaatsmq");
}

CCedaATSMQMng::~CCedaATSMQMng(void)
{
    if (m_hRegCedaSucessEvent) {
        CloseHandle(m_hRegCedaSucessEvent);
        m_hRegCedaSucessEvent = NULL;
    }
    if (m_hCedaSendRetEvent) {
        CloseHandle(m_hCedaSendRetEvent);
        m_hCedaSendRetEvent = NULL;
    }
    DeleteCriticalSection(&m_cs);

    /*
    if (m_pLogger) {
        delete m_pLogger;
        m_pLogger = NULL;
    }
    */
}

bool CCedaATSMQMng::InitCeda(CedaEventObs *pObs)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin InitCeda...\r\n");

    if (NULL == pObs) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "[InitCeda Failed]:CedaEventObs is NULL!\r\n");
        return false;
    }

    //init ssmq if it's not init
    if (!GetSSMQMngIns()->isConnectBroker()) {
        if (!GetSSMQMngIns()->InitServer()) {
            CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "[InitCeda Failed]:GetSSMQMngIns()->InitServer() failed!\r\n");
            return false;
        }
    }

    //reg ceda manage obs
    if (!GetSSMQMngIns()->RegCedaObs(this, CEDA_ATS_MQ_HEADER)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "[InitCeda Failed]:Reg ceda manage obs failed!\r\n");
        return false;
    }

    //reg ceda event obs
    if (!GetSSMQMngIns()->RegCedaObs(pObs, CEDA_ATS_MQ_HEADER)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "[InitCeda Failed]:Reg ceda event obs failed!\r\n");
        return false;
    }

    //reg ceda obs to server
    if (!RegCeda2Server(pObs)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "[InitCeda Failed]:RegCeda2Server failed!\r\n");
        return false;
    }

    m_bIsCedaInit = true;
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "InitCeda success.\r\n");
    return true;
}

void CCedaATSMQMng::LoginCeda(const sdbus::string &user, const sdbus::string &pwd)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin login ceda with user...\r\n");
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_LOGIN_WITH_USER);
    values.SetString(SSMQ_MSGIDX_CMDPARA, this->GetBindKey());
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, user);
    values.SetString(SSMQ_MSGIDX_CMDPARA+2, pwd);
    if (!GetSSMQMng()->Post(SSMQ_TOPIC_MNG, msg)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "login ceda with user failed!\r\n");
    }
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Login ceda with user end.\r\n");
}

void CCedaATSMQMng::LoginCeda(const sdbus::string &validKey)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin login ceda with json...\r\n");
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_LOGIN_WITH_JSON);
    values.SetString(SSMQ_MSGIDX_CMDPARA, this->GetBindKey());
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, validKey);
    if (!GetSSMQMng()->Post(SSMQ_TOPIC_MNG, msg)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "login ceda with json failed!\r\n");
    }
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Login ceda with json end.\r\n");
}

void CCedaATSMQMng::ReleaseCeda()
{
    bool ret = GetSSMQMngIns()->ReleaseCeda();
    if (ret) {
        m_bIsCedaInit = false;
    }
}

void CCedaATSMQMng::SetCedaMngCallback(ICedaATSMQMngCallback* pCallback)
{
    m_pMngCallback = pCallback;
}

void CCedaATSMQMng::CloseCedaDataBus()
{
    /*
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "CloseCedaDataBus.\r\n");
    sdbus::Message msg;
    msg.SetString(SSMQ_MSGIDX_CMDTYPE, SSMQ_CMD_CLOSEALL);
    msg.SetString(SSMQ_MSGIDX_CMDPARA, GetCedaClientName());
    Post(SSMQ_TOPIC_MNG, msg);
    */
}

/*
void CCedaATSMQMng::ReleaseCedaObs(ICedaATSMQObs* pObs)
{
    GetSSMQMngIns()->ReleaseCedaObs(pObs, CEDA_ATS_MQ_HEADER);
}
*/

void CCedaATSMQMng::ReleaseAllCedaObs()
{
    GetSSMQMngIns()->ReleaseAllCedaObs();
}

bool CCedaATSMQMng::Send(const sdbus::string &topic, const sdbus::string &request, sdbus::string &response, const sdbus::string &svrID, const int timeout/* = -1*/)
{
    m_hCedaSendRetEvent = CreateEventA(NULL, FALSE, FALSE, UNIQUE_ASYNC_SEND_SUCESS);
    sdbus::Message msgReq;
    sdbus::Message::Map& values = msgReq.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_CEDA_SEND);
    values.SetString(SSMQ_MSGIDX_CMDPARA, this->GetBindKey());
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, topic);
    values.SetString(SSMQ_MSGIDX_CMDPARA+2, request);
    values.SetString(SSMQ_MSGIDX_CMDPARA+3, svrID);
    if (!GetSSMQMngIns()->Post(SSMQ_TOPIC_MNG, msgReq)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "ceda send msg failed!\r\n");
        return false;
    }
    if (WAIT_TIMEOUT == WaitForSingleObject(m_hCedaSendRetEvent, /*INFINITE*/timeout)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "ceda send msg ack timeout!\r\n");
        return false;
    }
    response = m_sendBuf;
    m_sendBuf.clear();
    return true;
}

void CCedaATSMQMng::Post(const sdbus::string &topic, const sdbus::string &content)
{
    //todo
    /*
    sdbus::string cedaBindKey = getCedaBindKey(CEDA_ATS_MQ_HEADER, topic);
    GetSSMQMngIns()->Post(cedaBindKey, content);
    */
}

void CCedaATSMQMng::Subscribe(CedaTopicObs *pObs, const sdbus::string &topics, const sdbus::string &svrID)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin CCedaATSMQMng::Subscribe...\r\n");
    //reg ceda topic obs
    if (!GetSSMQMngIns()->RegCedaObs(pObs, CEDA_ATS_MQ_HEADER)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "[CCedaATSMQMng::Subscribe Failed]:Reg ceda topic obs failed!\r\n");
        return;
    }
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_CEDA_SUBSCRIBE);
    values.SetString(SSMQ_MSGIDX_CMDPARA, pObs->GetBindKey());
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, topics);
    values.SetString(SSMQ_MSGIDX_CMDPARA+2, svrID);
    if (!GetSSMQMng()->Post(SSMQ_TOPIC_MNG, msg)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "CCedaATSMQMng::Subscribe post failed!\r\n");
        return;
    }
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "CCedaATSMQMng::Subscribe end.\r\n");
}

void CCedaATSMQMng::unSubscribe(CedaTopicObs *pObs)
{
    //todo
}

sdbus::string CCedaATSMQMng::GetBindKey()
{
    sdbus::string sBindKey(CEDA_ATS_MQ_MANAGE);
    sBindKey += GET_GOL_CONF.sCedaBindKeySpr;
    sBindKey += GetCedaClientName();
    return sBindKey;
}

SSMQ::SSMQDeliverType CCedaATSMQMng::GetDeliverType()
{
    return SSMQ::SSMQDT_Direct;
}

void CCedaATSMQMng::OnMsgs(const sdbus::Message& msgs)
{
    sdbus::string sCmdType;
    msgs.GetString(SSMQ_MSGIDX_CMDTYPE, sCmdType);
    if (CEDA_CMD_CEDA_INIT_ACK == sCmdType) {
        bool bInitRet = false;
        msgs.GetBool(SSMQ_MSGIDX_CMDPARA, bInitRet);
        if (bInitRet) {
            SetEvent(m_hRegCedaSucessEvent);
        }
    }
    else if (CEDA_CMD_CLOSE_CLIENT == sCmdType) {
        std::string sClientName;
        msgs.GetString(SSMQ_MSGIDX_CMDPARA, sClientName);
        if (m_pMngCallback) {
            m_pMngCallback->CloseCedaClient();
        }
    }
    else if (CEDA_DISPATCH_RES_MSG == sCmdType) {
        msgs.GetString(SSMQ_MSGIDX_CMDPARA+1, m_sendBuf);
        SetEvent(m_hCedaSendRetEvent);
    }
}

sdbus::string CCedaATSMQMng::getCedaBindKey(const sdbus::string &cedaHeader, const sdbus::string &bindKey)
{
    sdbus::string cedaATSKey;
    cedaATSKey += cedaHeader;
    cedaATSKey += GET_GOL_CONF.sCedaBindKeySpr;
    cedaATSKey += bindKey;
    return cedaATSKey;
}

sdbus::string CCedaATSMQMng::GetCedaClientName()
{
    //procid作为CedaClientName
    char buf[32];
    DWORD dwPid = GetCurrentProcessId();
    sprintf_s(buf, 32, "%d", dwPid);
    return buf;
}

bool CCedaATSMQMng::RegCeda2Server(CedaEventObs *pObs)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin reg ceda to server...\r\n");
    m_hRegCedaSucessEvent = CreateEventA(NULL, FALSE, FALSE, UNIQUE_EVENTNAME_RGCEDASUCESS);
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_CEDA_INIT);
    values.SetString(SSMQ_MSGIDX_CMDPARA, this->GetBindKey());
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, pObs->GetBindKey());
    if (!GetSSMQMngIns()->Post(SSMQ_TOPIC_MNG, msg)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "Reg ceda to server failed!\r\n");
    }
    return WaitRegCedaSucess();
}

bool CCedaATSMQMng::IsCedaInit()
{
    return m_bIsCedaInit;
}

void CCedaATSMQMng::InitConfig()
{
    CConfig::getInstance();
}

void CCedaATSMQMng::UnRegCeda2Server()
{
    GetSSMQMngIns()->UnRegCeda2Server();
}

bool CCedaATSMQMng::WaitRegCedaSucess()
{
    if (WAIT_TIMEOUT == WaitForSingleObject(m_hRegCedaSucessEvent, /*INFINITE*/5000)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "Wait reg ceda ack timeout!\r\n");
        return false;
    }
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Reg ceda to server success.\r\n");
    return true;
}

sdbus::string CCedaATSMQMng::GetUniqueSendID()
{
    sdbus::string uniqueID;
    uniqueID = CEDA_ATS_MQ_SEND;
    uniqueID += GET_GOL_CONF.sCedaBindKeySpr;
    uniqueID += GetCedaATSMQMngIns()->GetUUID();
    uniqueID += GET_GOL_CONF.sCedaBindKeySpr;;
    uniqueID += GetCedaATSMQMngIns()->GetCedaClientName();
    return uniqueID;
}

std::string CCedaATSMQMng::GetUUID()
{
    std::string sUUID;
    GUID guid;
    if (!CoCreateGuid(&guid)) {
        char buffer[64] = { 0 };
        _snprintf_s(buffer, sizeof(buffer),
            //"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",    //大写
            "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",  //小写
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1], guid.Data4[2],
            guid.Data4[3], guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);
        sUUID = buffer;
    }
    return sUUID;
}

CEDA_ATSMQCLIENT_API sdbus::string CedaEventObs::GetBindKey()
{
    sdbus::string sBindKey(CEDA_ATS_MQ_EVENT);
    sBindKey += GET_GOL_CONF.sCedaBindKeySpr;
    sBindKey += GetCedaATSMQMngIns()->GetCedaClientName();
    return sBindKey;
}

CEDA_ATSMQCLIENT_API SSMQ::SSMQDeliverType CedaEventObs::GetDeliverType()
{
    return SSMQ::SSMQDT_Direct;
}

CEDA_ATSMQCLIENT_API void CedaEventObs::OnMsgs(const sdbus::Message& msgs)
{
    sdbus::string sCmdType;
    msgs.GetString(SSMQ_MSGIDX_CMDTYPE, sCmdType);
    if (CEDA_CMD_LOGIN_ACK == sCmdType) {
        int nEventID = 0;
        std::string sid;
        msgs.GetInt32(SSMQ_MSGIDX_CMDPARA, nEventID);
        msgs.GetString(SSMQ_MSGIDX_CMDPARA+1, sid);
        this->OnEvernt(nEventID, sid);
    }
    else if (CEDA_CMD_CEDA_SUBSCRIBE_ACK == sCmdType) {
        int nEventID = 0;
        std::string retMsg;
        msgs.GetInt32(SSMQ_MSGIDX_CMDPARA, nEventID);
        msgs.GetString(SSMQ_MSGIDX_CMDPARA+1, retMsg);
        this->OnEvernt(nEventID, retMsg);
    }
    else if (CEDA_CMD_CEDA_UNSUBSCRIBE_ACK == sCmdType) {
    }
}

CEDA_ATSMQCLIENT_API CedaTopicObs::CedaTopicObs()
{
    m_obsBindKey = CEDA_ATS_MQ_TOPIC;
    m_obsBindKey += GET_GOL_CONF.sCedaBindKeySpr;
    m_obsBindKey += GetCedaATSMQMngIns()->GetUUID();
    m_obsBindKey += GET_GOL_CONF.sCedaBindKeySpr;;
    m_obsBindKey += GetCedaATSMQMngIns()->GetCedaClientName();
}

CEDA_ATSMQCLIENT_API sdbus::string CedaTopicObs::GetBindKey()
{
    return m_obsBindKey;
}

CEDA_ATSMQCLIENT_API SSMQ::SSMQDeliverType CedaTopicObs::GetDeliverType()
{
    return SSMQ::SSMQDT_Direct;
}

CEDA_ATSMQCLIENT_API void CedaTopicObs::OnMsgs(const sdbus::Message& msgs)
{
    sdbus::string sCmdType;
    msgs.GetString(SSMQ_MSGIDX_CMDTYPE, sCmdType);
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_DEBUG, "sCmdType:[%s].\r\n", sCmdType.c_str());
    if (CEDA_DISPATCH_RES_MSG == sCmdType) {
        std::string topic;
        std::string content;
        int len = 0;
        msgs.GetString(SSMQ_MSGIDX_CMDPARA, topic);
        msgs.GetString(SSMQ_MSGIDX_CMDPARA+1, content);
        msgs.GetInt32(SSMQ_MSGIDX_CMDPARA+2, len);
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_DEBUG, "topic:[%s], len:[%d]\r\n", topic.c_str(), len);
        this->onMessage(topic, content, len);
    }
}