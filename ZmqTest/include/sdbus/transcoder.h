

#ifndef __SDBUS_TRANSCODER_H
#define __SDBUS_TRANSCODER_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/string.h"


namespace google{
namespace protobuf{
class Message ;
}
}

namespace sdbus{

/*
    ����һ��protobuf����Ϣ��sdbus��Ϣ֮���ת������
    �������ӻ��߱��protobuf����Ϣ����Ҫ���±���sdbus��

    ����ֻ࣬����google::protobuf::Message��sdbus::Message֮��ֵ�Ŀ�����
*/
class SDBUSAPI MsgTranscoder{
public:
    MsgTranscoder() ;
    virtual ~MsgTranscoder() {}

    virtual bool FromProtobuf(Message& msg , const google::protobuf::Message& protobuf) const {return false ;}

    virtual bool ToProtobuf(const Message& msg , const sdbus::string& class_name , google::protobuf::Message*& protobuf) const {return false ;}

    int AppID ;

    static const sdbus::string kFIXCMsgName ;
} ;

/*
    Ĭ�ϵģ�ת��ͽ���FIXC.Message������Ҫ�ж�AppID��
*/
class SDBUSAPI FIXCTranscoder : public MsgTranscoder{
public:
    bool FromProtobuf(Message& msg , const google::protobuf::Message& protobuf) const ;

    bool ToProtobuf(const Message& msg , const sdbus::string& class_name , google::protobuf::Message*& protobuf) const ;

    static const FIXCTranscoder& Singleton() ;
} ;


SDBUSAPI bool RegisterMsgTranscoder(int appid , MsgTranscoder * transcoder) ;

SDBUSAPI const MsgTranscoder * FindMsgTranscoder(int appid) ;

} 

#endif  /** __SDBUS_TRANSCODER_H */


