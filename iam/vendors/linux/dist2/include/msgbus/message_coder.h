
#ifndef __MSGBUS_MESSAGE_CODER_H
#define __MSGBUS_MESSAGE_CODER_H 1

#include "msgbus/variant.h"
#include <string>

namespace msgbus{


/*
    写入的过程中，不包含类型，因为需要根据类型来决定调用哪个函数
*/
class MSGBUSAPI Buffer{
public:
    Buffer(void * data = 0 , uint32_t size = 0) ;

    void Reset() ;
    inline uint32_t Available() const {return size_ - offset_ ;}
    inline uint32_t Size() const {return size_ ; }
    inline uint32_t Offset() const {return offset_ ;}
    inline void Offset(uint32_t val) {offset_ = val ;}
    inline char * Data() const {return data_ ;}

    inline bool CheckSize(size_t len) const {return check_size(len) ;}

    inline void Shrink(uint32_t val) 
    {
        if(val > offset_)
            offset_ = 0 ;
        offset_ -= val ;
    }

    /*
        设置值，基础类型
    */
    bool PutUInt8(uint8_t val) ;
    bool PutUInt16(uint16_t val) ;
    bool PutUInt32(uint32_t val) ;
    bool PutUInt64(uint64_t val) ;

    bool PutInt8(int8_t val) ;
    bool PutInt16(int16_t val) ;
    bool PutInt32(int32_t val) ;
    bool PutInt64(int64_t val) ;

    bool PutFloat(float val) ;
    bool PutDouble(double val) ;


    /*
        取值，基础类型
    */
    bool GetUInt8(uint8_t& val) ;
    bool GetUInt16(uint16_t& val) ;
    bool GetUInt32(uint32_t& val) ;
    bool GetUInt64(uint64_t& val) ;

    bool GetInt8(int8_t& val) ;
    bool GetInt16(int16_t& val) ;
    bool GetInt32(int32_t& val) ;
    bool GetInt64(int64_t& val) ;

    bool GetFloat(float& val) ;
    bool GetDouble(double& val) ;


    /*
        设置值，变长类型，包括长度
    */
    bool PutString(const std::string& val) ;       //长度1个字节表示，最大255
    bool PutShortString(const std::string& val) ;  //长度2个字节表示，最大65535
    bool PutLongString(const std::string& val) ;   //最长4个字节表示，最大...

    bool PutRaw(const void * data , size_t size) ;
    bool PutShortRaw(const void * data , size_t size) ;
    bool PutLongRaw(const void * data , size_t size) ;
    

    /*
        取值，变长类型，拷贝到string类中
    */
    bool GetString(std::string& val) ;       //长度1个字节表示，最大255
    bool GetShortString(std::string& val) ;  //长度2个字节表示，最大65535
    bool GetLongString(std::string& val) ;   //最长4个字节表示，最大...

    /*
        取值，不拷贝。
    */
    bool GetString(const char *& val , size_t& size) ;
    bool GetShortString(const char *& val , size_t& size) ;
    bool GetLongString(const char *& val , size_t& size) ;

    bool GetRaw(const void *& data , size_t& size) ;
    bool GetShortRaw(const void *& data , size_t& size) ;
    bool GetLongRaw(const void *& data , size_t& size) ;

private:
    uint32_t size_ ;
    uint32_t offset_ ;
    char * data_ ;

    bool check_size(size_t len) const;
} ;

class MSGBUSAPI Encoder
{
public:
    explicit Encoder(Buffer& buffer) ;

    bool ToQpidType(const Variant& var , uint8_t& type) ;

    bool Encode(const Variant& var) ;

    bool EncodeBool(bool value) ;

    bool EncodeInt8(int8_t value) ;
    bool EncodeUInt8(uint8_t value) ;
    bool EncodeInt16(int16_t value) ;
    bool EncodeUInt16(uint16_t value) ;
    bool EncodeInt32(int32_t value) ;
    bool EncodeUInt32(uint32_t value) ;
    bool EncodeInt64(int64_t value) ;
    bool EncodeUInt64(uint64_t value) ;
    
    bool EncodeFloat(float value) ;
    bool EncodeDouble(double value) ;

    bool EncodeString(const char * value , size_t size) ;
    bool EncodeRaw(const void * value , size_t size) ;

    bool EncodeMap(const Variant::Map& value) ;
    bool EncodeList(const Variant::List& value) ;
    bool EncodeVoid(const Variant& var) ;


    /*
        定长的字段，都是type + value
    */

    static int CalcSize(const Variant& var) ;

    static int CalcSizeBool(bool value) ;

    static int CalcSizeInt8(int8_t value) ;
    static int CalcSizeUInt8(uint8_t value) ;
    static int CalcSizeInt16(int16_t value) ;
    static int CalcSizeUInt16(uint16_t value) ;
    static int CalcSizeInt32(int32_t value) ;
    static int CalcSizeUInt32(uint32_t value) ;
    static int CalcSizeInt64(int64_t value) ;
    static int CalcSizeUInt64(uint64_t value) ;
    
    static int CalcSizeFloat(float value) ;
    static int CalcSizeDouble(double value) ;

    /*
        变长的字段，都是type + size + value
    */
    static int CalcSizeString(const char * value , size_t size) ;
    static int CalcSizeRaw(const void * value , size_t size) ;

    static int CalcSizeMap(const Variant::Map& value) ;
    static int CalcSizeList(const Variant::List& value) ;
    static int CalcSizeVoid(const Variant& var) ;
private:
    Buffer& buffer_ ;
} ;


class MSGBUSAPI Decoder
{
public:
    explicit Decoder(Buffer& buffer) ;

    bool Decode(Variant& var) ;

    bool DecodeType(uint8_t& type) ;

    bool DecodeBool(Variant& var) ;

    bool DecodeInt8(Variant& var) ;
    bool DecodeUInt8(Variant& var) ;
    bool DecodeInt16(Variant& var) ;
    bool DecodeUInt16(Variant& var) ;
    bool DecodeInt32(Variant& var) ;
    bool DecodeUInt32(Variant& var) ;
    bool DecodeInt64(Variant& var) ;
    bool DecodeUInt64(Variant& var) ;

    bool DecodeFloat(Variant& var) ;
    bool DecodeDouble(Variant& var) ;

    bool DecodeString(Variant& var) ;
    bool DecodeShortString(Variant& var) ;

    bool DecodeRaw(Variant& var) ;
    bool DecodeShortRaw(Variant& var) ;
    bool DecodeLongRaw(Variant& var) ;

    bool DecodeMap(Variant& var) ;
    bool DecodeList(Variant& var) ;
    bool DecodeVoid(Variant& var) ;

    bool DecodeMapDirect(Variant::Map& map) ;
private:
    Buffer& buffer_ ;
} ;


}

#endif /** __MSGBUS_MESSAGE_CODER_H */
