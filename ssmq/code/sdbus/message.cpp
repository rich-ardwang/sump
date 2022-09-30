
#include "sdbus/message.h"
#include "SSRTL/SSLog.h"

namespace sdbus{

Message::Message()
{
    total_messages_ = 1 ;
    message_number_ = 0 ;
}

Message::~Message()
{
    //
}

Message::Message(const Message& msg) 
{
    total_messages_ = 1 ;
    message_number_ = 0 ;

    Assign(msg) ;
}

Message& Message::operator=(const Message& msg)
{
    Assign(msg) ;
    return (*this) ;
}

bool Message::GetFieldInfo(int index , uint32_t& tag , int& type) const 
{
    return  map_values_.GetFieldInfo(index , tag , type) ;
}

bool Message::GetFieldInfo(int index , sdbus::string& name , int& type) const 
{
    return map_values_.GetFieldInfo(index , name , type) ;
}

bool Message::GetFieldValue(int index , const Variant *& var) const 
{
    return map_values_.GetFieldValue(index , var) ;
}

bool Message::SetBool(uint32_t tag, bool value)
{
    return map_values_.SetBool(tag , value) ;
}

bool Message::SetUInt8(uint32_t tag, uint8_t value)
{
    return map_values_.SetUInt8(tag , value) ;
}

bool Message::SetUInt16(uint32_t tag, uint16_t value)
{
    return map_values_.SetUInt16(tag , value) ;
}

bool Message::SetUInt32(uint32_t tag, uint32_t value)
{
    return map_values_.SetUInt32(tag , value) ;
}

bool Message::SetUInt64(uint32_t tag, uint64_t value)
{
    return map_values_.SetUInt64(tag , value) ;
}

bool Message::SetInt8(uint32_t tag, int8_t value)
{
    return map_values_.SetInt8(tag , value) ;
}

bool Message::SetInt16(uint32_t tag, int16_t value)
{
    return map_values_.SetInt16(tag , value) ;
}

bool Message::SetInt32(uint32_t tag, int32_t value)
{
    return map_values_.SetInt32(tag , value) ;
}

bool Message::SetInt64(uint32_t tag, int64_t value)
{
    return map_values_.SetInt64(tag , value) ;
}

bool Message::SetFloat(uint32_t tag, float value)
{
    return map_values_.SetFloat(tag , value) ;
}

bool Message::SetDouble(uint32_t tag, double value)
{
    return map_values_.SetDouble(tag , value) ;
}

bool Message::SetString(uint32_t tag, const char * value)
{
    return map_values_.SetString(tag , value) ;
}

bool Message::SetString(uint32_t tag, const sdbus::string& value)
{
    return map_values_.SetString(tag , value) ;
}

bool Message::SetRaw(uint32_t tag, const char *value, size_t size)
{
    return map_values_.SetRaw(tag , value , size) ;
}

bool Message::SetRaw(uint32_t tag, const sdbus::string&value)
{
    return map_values_.SetRaw(tag , value) ;
}

bool Message::SetMap(uint32_t tag , const Map& map) 
{
    return map_values_.SetMap(tag , map) ;
}

bool Message::SetList(uint32_t tag , const List& list) 
{
    return map_values_.SetList(tag , list) ;
}


VariantMap* Message::AddMap(uint32_t tag) 
{
    return map_values_.AddMap(tag) ;
}

VariantList* Message::AddList(uint32_t tag) 
{
    return map_values_.AddList(tag) ;
}

Variant* Message::AddVariant(uint32_t tag)
{
    return map_values_.AddVariant(tag) ;
}

bool Message::GetBool(uint32_t tag, bool &value) const
{
    return map_values_.GetBool(tag , value) ;
}

bool Message::GetUInt8(uint32_t tag, uint8_t &value) const
{
    return map_values_.GetUInt8(tag , value) ;
}

bool Message::GetUInt16(uint32_t tag, uint16_t &value) const
{
    return map_values_.GetUInt16(tag , value) ;
}

bool Message::GetUInt32(uint32_t tag, uint32_t &value) const
{
    return map_values_.GetUInt32(tag , value) ;
}

bool Message::GetUInt64(uint32_t tag, uint64_t &value) const
{
    return map_values_.GetUInt64(tag , value) ;
}

bool Message::GetInt8(uint32_t tag, int8_t &value) const
{
    return map_values_.GetInt8(tag , value) ;
}

bool Message::GetInt16(uint32_t tag, int16_t &value) const
{
    return map_values_.GetInt16(tag , value) ;
}

bool Message::GetInt32(uint32_t tag, int32_t &value) const
{
    return map_values_.GetInt32(tag , value) ;
}

bool Message::GetInt64(uint32_t tag, int64_t &value) const
{
    return map_values_.GetInt64(tag , value) ;
}

bool Message::GetFloat(uint32_t tag, float &value) const
{
    return map_values_.GetFloat(tag , value) ;
}

bool Message::GetDouble(uint32_t tag, double &value) const
{
    return map_values_.GetDouble(tag , value) ;
}

bool Message::GetString(uint32_t tag, sdbus::string& value) const
{
    return map_values_.GetString(tag , value) ;
}

bool Message::GetRaw(uint32_t tag, sdbus::string& value) const
{
    return map_values_.GetRaw(tag , value) ;
}

bool Message::GetMap(uint32_t tag, Map&value) const
{
    return map_values_.GetMap(tag , value) ;
}

bool Message::GetList(uint32_t tag , List& value) const
{
    return map_values_.GetList(tag , value) ;
}


bool Message::RefString(uint32_t tag, const char *& value, size_t &size) const
{
    return map_values_.RefString(tag , value , size) ;
}

bool Message::RefString(uint32_t tag, const sdbus::string*& value) const
{
    return map_values_.RefString(tag , value) ;
}

bool Message::RefRaw(uint32_t tag, const void *& value, size_t &size) const
{
    return map_values_.RefRaw(tag , value , size) ;
}

bool Message::RefRaw(uint32_t tag, const sdbus::string* value) const
{
    return map_values_.RefRaw(tag , value) ;
}

bool Message::RefVar(uint32_t tag, const Variant* &value) const
{
    return map_values_.RefVar(tag , value) ;
}

bool Message::RefMap(uint32_t tag, const Map* &value) const
{
    return map_values_.RefMap(tag , value) ;
}

bool Message::RefList(uint32_t tag , const List*& value) const
{
    return map_values_.RefList(tag , value) ;
}

bool Message::RefVar(uint32_t tag, Variant* &value) 
{
    return map_values_.RefVar(tag , value) ;
}

bool Message::RefMap(uint32_t tag, Map* &value) 
{
    return map_values_.RefMap(tag , value) ;
}

bool Message::RefList(uint32_t tag , List*& value)
{
    return map_values_.RefList(tag , value) ;
}

void Message::Clear()
{
    type_.clear() ;
    subject_.clear() ;
    msg_id_.clear() ;
    reply_to_.clear() ;

    total_messages_ = 0 ;
    message_number_ = 0 ;

    map_values_.Clear() ;
}

void Message::Attach(Message& msg) 
{
    type_ = msg.GetType() ;
    subject_ = msg.GetSubject() ;
    msg_id_ = msg.GetMessageID() ;
    reply_to_ = msg.GetReplyTo() ;

    total_messages_ = msg.total_messages_ ;
    message_number_ = msg.message_number_ ;

    map_values_.Attach(msg.MapValue()) ;
}

void Message::Assign(const Message& msg) 
{
    type_ = msg.type_ ;
    subject_ = msg.subject_ ;
    msg_id_ = msg.msg_id_ ;
    reply_to_ = msg.reply_to_ ;

    total_messages_ = msg.total_messages_ ;
    message_number_ = msg.message_number_ ;

    map_values_ = msg.map_values_ ;
}

}

