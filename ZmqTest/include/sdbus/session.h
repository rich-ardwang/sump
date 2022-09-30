
#ifndef __SDBUS_SESSION_H
#define __SDBUS_SESSION_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include "sdbus/message.h"
#include "sdbus/safe_object.h"
#include "sdbus/callback.h"

namespace sdbus{

class SessObjImpl ;
class SDBUSAPI SessObj {
public:
    SessObj() ;
    virtual ~SessObj() ;

    virtual uint32_t ObjID() const ;

    inline uint32_t ClientID() const {return client_id_ ;}
    inline void ClientID(uint32_t cid) {client_id_ = cid;}

    inline uint32_t ClientSequence() const {return client_sequence_ ;}
    inline void ClientSequence(uint32_t cseq) {client_sequence_ = cseq ;}

    inline uint32_t ServiceID() const {return service_id_ ;}
    inline void ServiceID(uint32_t sid) {service_id_ = sid;}

    inline uint32_t ServiceSequence() const {return service_sequence_ ;}
    inline void ServiceSequence(uint32_t sseq) {service_sequence_ = sseq ;}

    inline uint32_t SourceAddress() const {return source_address_ ;}
    inline void SourceAddress(uint32_t addr) {source_address_ = addr ;}

    inline uint32_t DestAddress() const {return dest_address_ ;}
    inline void DestAddress(uint32_t addr ) {dest_address_ = addr;}

    inline const sdbus::string& SessionID() const {return session_id_ ;}
    inline void SessionID(const sdbus::string& sid) {session_id_ = sid ;}

    inline bool AutoDelete() const {return autodelete_ ;}
    inline void AutoDelete(bool ad) {autodelete_ = ad ;}

    inline bool AutoCreate() const {return autocreate_ ;}
    inline void AutoCreate(bool ac) {autocreate_ = ac ;}

    long increment() const;
    long decrement() const;

    bool GenerateSessionStatus(SessionStatus& status) const;

private:

    uint32_t client_id_ ;
    uint32_t client_sequence_ ;
    uint32_t service_id_ ;
    uint32_t service_sequence_ ;

    uint32_t source_address_ ;
    uint32_t dest_address_ ;             //��������ڵĵ�ַ

    sdbus::string session_id_ ;

    //���Ự�Ͽ�ʱ���Զ�ɾ����Ĭ��Ϊ�١������Ҫ������ʱ�Ự����Ҫ�ֹ����á�
    bool autodelete_ ;

    //���ͻ��˷��𴴽�ָ��ʱ���Զ�������Ĭ��Ϊ�档
    bool autocreate_ ;

    SessObjImpl * impl_ ;
} ;

typedef SafeRef<SessObj> SessObjRef ;


/*
    �ͻ��˺ͷ����֮���Ԥ����FID����Ҫ���ڻỰָ�
    ����ǰ������Ϣ���͵��޶������Բ���Ҫ���ĺ������ظ���
*/
static const uint32_t TAG_RETURN_CODE       =   2075   ;
static const uint32_t TAG_ERROR_MESSAGE     =   2076   ;

static const uint32_t TAG_OPTION_AUTODELETE =   999991 ;
static const uint32_t TAG_OPTION_AUTOCREATE =   999992 ;

/*
    �ͻ���
*/
class ClientManager ;
class SDBUSAPI Client : public SessObj{
public:
    Client() ;
    virtual ~Client() ;

    virtual bool Init() ;
    virtual bool Final() ;

    virtual void OnMessage(const Message& msg) ;

    inline uint32_t ObjID() const {return ClientID() ;}

    bool Send(const Message& msg) ;

    /*
        ����ʽ����timeout��λΪ�롣���С��0����ôΪ���޳�ʱ��
    */
    bool Send(const Message& req , Message& rsp , int timeout = -1) ;

    /*
        �󶨵������һ���Ự����sessΪ�ؼ��֡�
        ���ݷ���˵ĻỰ���������صĲ���������ʼ�����ز�����

        ���addrΪ0����ô���÷�����Զ�ѡ��
        ���sessΪ�գ���ô��ǿ�Ʊ�����Ϊ��ʱ�Ự��
    */
    bool BindSession(uint32_t addr , const sdbus::string& sess) ;
    bool UnbindSession() ;

    inline const ClientManager * Owner() const {return owner_ ;}
    inline ClientManager * Owner() {return owner_ ;}
    inline void Owner(ClientManager * owner) {owner_ = owner ;}

private:
    ClientManager * owner_ ;
} ;

typedef SessObjRef ClientRef ;
class SessObjManagerImpl ;
class SDBUSAPI ClientManager : public Callback{
public:
    ClientManager() ;
    virtual ~ClientManager() ;

    virtual bool Init(int appid , const sdbus::string& name) ;
    virtual void Final() ;

    virtual void OnMsgs(const Message ** msgs , int count) ;

    bool Send(const Message& msg) ;

    bool Send(const Message& req , Message& rsp , int timeout = -1) ;

    int Count() const ;
    size_t List(ClientRef * refs , size_t count) const ;

    ClientRef FindByCliID(uint32_t cid) const ;
    ClientRef FindBySessID(const sdbus::string& sid) const ;

    /*
        ���Clientû�����ñ��ص�ClientID��ClientSequence��
        ��ô��AddClientʱ�����Զ���ֵ��
    */
    bool AddClient(Client * cli) ;
    void DelClient(const ClientRef& ref) ;
    void DelClient(uint32_t cid) ;
    void DelClient(const sdbus::string& sid) ;

    void Clear() ;

    /*
        ����һ�����ڱ����ظ���ClientID
    */
    uint32_t GenerateClientID() const ;

private:
    SessObjManagerImpl * objs_ ;
} ;


/*
    ����һ����0��������к�
*/
SDBUSAPI uint32_t GenerateRandSequence() ;

/*
    �����
*/
class SessionManager ;
class SDBUSAPI Session : public SessObj{
public:
    Session() ;
    virtual ~Session() ;

    virtual bool Init() ;
    virtual bool Final() ;

    virtual uint32_t ObjID() const {return ServiceID() ;}

    virtual void OnMessage(const Message& msg) ;

    bool Send(const sdbus::string& dest , const Message& msg) ;

    /*
        ���ڻỰֻ��Ӧ�𣬲�Ӧ�ô���ͬ��ģʽ�����Բ�֧�֡�
    */

    inline const SessionManager * Owner() const {return owner_ ;}
    inline SessionManager * Owner() {return owner_ ;}
    inline void Owner(SessionManager * owner) {owner_ = owner ;}

private:
    SessionManager * owner_ ;
} ;

typedef SessObjRef SessionRef ;

class SDBUSAPI SessionManager : public Callback{
public:
    SessionManager() ;
    virtual ~SessionManager() ;

    typedef Session * (*SessionCreator)(SessionManager *  owner) ;

    virtual bool Init(int appid , const sdbus::string& name , SessionCreator creator) ;
    virtual void Final() ;

    virtual void OnMsgs(const Message ** msgs , int count) ;

    bool Send(const sdbus::string& dest , const Message& msg) ;

    int Count() const ;
    size_t List(SessionRef * refs , size_t count) const ;

    SessionRef FindBySvcID(uint32_t sid) const ;
    SessionRef FindBySessID(const sdbus::string& sid) const ;

    /*
        ���Clientû�����ñ��ص�ClientID��ClientSequence��
        ��ô��AddClientʱ�����Զ���ֵ��
    */
    bool AddSession(Session * sess) ;
    void DelSession(const SessionRef& ref) ;
    void DelSession(uint32_t sid) ;
    void DelSession(const sdbus::string& sid) ;

    void Clear() ;

    /*
        ����һ�����ڱ����ظ���ServiceID
    */
    uint32_t GenerateServiceID() const ;

private:
    SessObjManagerImpl * objs_ ;
    SessionCreator creator_ ;

    void OnSessionBind(const Message& msg) ;
    void OnSessionUnbind(const Message& msg) ;
    void OnSessionProcess(const Message& msg) ;

    void ReportError(const Message& msg , int retcode , const sdbus::string& errmsg) ;
} ;

}


#endif  /** __SDBUS_SESSION_H */

