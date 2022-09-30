
#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/variant.h"
#include "sdbus/codec.h"
#include "impl/codec_impl.h"


namespace sdbus{


bool VariantMapCalcSize(const VariantMap& map , size_t& size)
{
    Variant var ;
    var.Type = Variant::VAR_MAP ;
    var.Data.map = (VariantMap *)&map ;
    var.Flag = 1 ;
    int len = var_map_sizer(&var , 0 , false) ;
    var.Data.map = NULL ;
    if(len < 0)
        return false ;

    size = len ;
    return true ;
}

bool VariantMapEncode(const VariantMap& map , char * buffer , size_t buflen , size_t& used)
{
    Variant var ;
    var.Type = Variant::VAR_MAP ;
    var.Data.map = (VariantMap *)&map ;
    var.Flag = 1 ;
    int len = var_map_encoder(&var , buffer , buflen , 0 , false) ;
    var.Data.map = NULL ;
    if(len < 0)
        return false ;

    used = len ;
    return true ;
}

bool VariantMapDecode(VariantMap& map , const char * buffer , size_t buflen , size_t& used)
{
    used = 0 ;
    if(buffer == NULL || buflen <= 0)
        return true ;

    int type = 0 ;
    ::memcpy(&type , buffer , 1) ;
    int offset = 1 ;

    if(type != Variant::VAR_MAP)
        return false ;

    Variant var ;
    var.Type= Variant::VAR_MAP ;

    int len = var_map_decoder(&var , buffer + offset , buflen - offset) ;    
    if(len < 0)
        return false ;

    var.Flag = 1 ;

    VariantMap * result = NULL ;
    if(var.RefMap(result) == false || result == false)
        return false ;

    map.Attach(*result) ;

    offset += len ;
    used = offset ;
    return true ;
}

bool VariantListCalcSize(const VariantList& list , size_t& size)
{
    Variant var ;
    var.Type = Variant::VAR_LIST ;
    var.Data.list = (VariantList *)&list ;
    var.Flag = 1 ;
    int len = var_list_sizer(&var , 0 , false) ;
    var.Data.list = NULL ;
    if(len < 0)
        return false ;

    size = len ;
    return true ;
}

bool VariantListEncode(const VariantList& list , char * buffer , size_t buflen , size_t& used)
{
    Variant var ;
    var.Type = Variant::VAR_LIST ;
    var.Data.list = (VariantList *)&list ;
    var.Flag = 1 ;
    int len = var_list_encoder(&var , buffer , buflen , 0 , false) ;
    var.Data.list = NULL ;
    if(len < 0)
        return false ;

    used = len ;
    return true ;
}

bool VariantListDecode(VariantList& list , const char * buffer , size_t buflen , size_t& used)
{
    used = 0 ;
    if(buffer == NULL || buflen <= 0)
        return true ;

    int type = 0 ;
    ::memcpy(&type , buffer , 1) ;
    int offset = 1 ;

    if(type != Variant::VAR_LIST)
        return false ;

    Variant var ;
    var.Type = Variant::VAR_LIST ;

    int len = var_list_decoder(&var , buffer + offset , buflen - offset) ;    
    if(len < 0)
        return false ;

    var.Flag = 1 ;

    VariantList * result = NULL ;
    if(var.RefList(result) == false || result == false)
        return false ;

    list.Attach(*result) ;

    offset += len ;
    used = offset ;
    return true ;
}


bool VariantRawCalcSize(const sdbus::string& raw , size_t& size)
{
    Variant var ;
    var.Type = Variant::VAR_RAW ;
    var.Data.raw = (sdbus::string *)&raw ;
    var.Flag = 1 ;
    int len = var_raw_sizer(&var , 0 , false) ;
    var.Data.raw = NULL ;
    if(len < 0)
        return false ;

    size = len ;
    return true ;
}

bool VariantRawEncode(const sdbus::string& raw , char * buffer , size_t buflen , size_t& used)
{
    Variant var ;
    var.Type = Variant::VAR_RAW ;
    var.Data.raw = (sdbus::string *)&raw ;
    var.Flag = 1 ;
    int len = var_raw_encoder(&var , buffer , buflen , 0 , false) ;
    var.Data.raw = NULL ;
    if(len < 0)
        return false ;

    used = len ;
    return true ;
}

bool VariantRawDecode(sdbus::string& raw , const char * buffer , size_t buflen , size_t& used)
{
    used = 0 ;
    if(buffer == NULL || buflen <= 0)
        return true ;

    int type = 0 ;
    ::memcpy(&type , buffer , 1) ;
    int offset = 1 ;

    if(type != Variant::VAR_RAW)
        return false ;

    Variant var ;

    int len = var_raw_decoder(&var , buffer + offset , buflen - offset) ;    
    if(len < 0)
        return false ;

    sdbus::string * result = NULL ;
    if(var.RefString(result) == false || result == false)
        return false ;

    raw.attach(*result) ;

    offset += len ;
    used = offset ;
    return true ;
}

}



