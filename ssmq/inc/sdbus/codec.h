
#ifndef __SDBUS_CODEC_H
#define __SDBUS_CODEC_H 1


#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/variant.h"


namespace sdbus{


/*
    �ڴ��ʽΪTag + Type + Value

    ����List��Map��˵�� Value = Size + Bytes ;

    ��List�ڲ���ÿ��Ԫ��ΪType + Value�����TypeΪ�������ͣ���ôValue��ȷ���ġ�
    ���TypeΪ�������ͣ���ôValue����һ��Size + Bytes ��

    ����Map�ڲ���ÿ����Ա������Tag + Type + Value
*/

SDBUSAPI bool VariantMapCalcSize(const VariantMap& map , size_t& size) ;

SDBUSAPI bool VariantMapEncode(const VariantMap& map , char * buffer , size_t buflen , size_t& used) ;

SDBUSAPI bool VariantMapDecode(VariantMap& map , const char * buffer , size_t buflen , size_t& used) ;


SDBUSAPI bool VariantListCalcSize(const VariantList& list , size_t& size) ;

SDBUSAPI bool VariantListEncode(const VariantList& list , char * buffer , size_t buflen , size_t& used) ;

SDBUSAPI bool VariantListDecode(VariantList& list , const char * buffer , size_t buflen , size_t& used) ;


SDBUSAPI bool VariantRawCalcSize(const sdbus::string& raw , size_t& size) ;

SDBUSAPI bool VariantRawEncode(const sdbus::string& raw , char * buffer , size_t buflen , size_t& used) ;

SDBUSAPI bool VariantRawDecode(sdbus::string& raw , const char * buffer , size_t buflen , size_t& used) ;

}

#endif  /** __SDBUS_CODEC_H */
