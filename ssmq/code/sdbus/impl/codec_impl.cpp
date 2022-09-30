
#include "codec_impl.h"

namespace sdbus{

typedef int (*variant_encoder_t)(const sdbus::Variant * var , char * buffer , size_t buflen , uint32_t tag  , bool withtag) ; 

typedef int (*variant_decoder_t)(sdbus::Variant * var , const char * buffer , size_t buflen) ;

typedef int (*variant_sizer_t)(const sdbus::Variant * var , uint32_t tag  , bool withtag) ; 

variant_encoder_t ves[Variant::VAR_MAX + 1] = {
    var_void_encoder , 
    var_bool_encoder , 

    var_int8_encoder , 
    var_uint8_encoder , 

    var_int16_encoder , 
    var_uint16_encoder , 

    var_int32_encoder , 
    var_uint32_encoder , 

    var_int64_encoder , 
    var_uint64_encoder , 

    var_float_encoder , 
    var_double_encoder , 

    var_string_encoder , 
    var_raw_encoder , 

    var_map_encoder , 
    var_list_encoder 
} ;

variant_sizer_t vss[Variant::VAR_MAX + 1] = {

    var_void_sizer , 
    var_bool_sizer , 

    var_int8_sizer , 
    var_uint8_sizer , 

    var_int16_sizer , 
    var_uint16_sizer , 

    var_int32_sizer , 
    var_uint32_sizer , 

    var_int64_sizer , 
    var_uint64_sizer , 

    var_float_sizer , 
    var_double_sizer , 

    var_string_sizer , 
    var_raw_sizer , 

    var_map_sizer , 
    var_list_sizer 

} ;


variant_decoder_t vds[Variant::VAR_MAX + 1] = {
    var_void_decoder , 
    var_bool_decoder , 

    var_int8_decoder , 
    var_uint8_decoder , 

    var_int16_decoder , 
    var_uint16_decoder , 

    var_int32_decoder , 
    var_uint32_decoder , 

    var_int64_decoder , 
    var_uint64_decoder , 

    var_float_decoder , 
    var_double_decoder , 

    var_string_decoder , 
    var_raw_decoder , 

    var_map_decoder , 
    var_list_decoder 
} ;

int var_void_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag)
{
    return 0 ;
}

int var_bool_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.bv , 1) ;
    offset += 1 ;

    return offset ;
}

int var_int8_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag)
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.i8 , 1) ;
    offset += 1 ;

    return offset ;
}

int var_uint8_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag)
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.u8 , 1) ;
    offset += 1 ;

    return offset ;
}

int var_int16_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    } 

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.i16 , 2) ;
    offset += 2 ;

    return offset ;
}

int var_uint16_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.u16 , 2) ;
    offset += 2 ;

    return offset ;
}

int var_int32_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.i32 , 4) ;
    offset += 4 ;

    return offset ;
}

int var_uint32_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.u32 , 4) ;
    offset += 4 ;

    return offset ;
}

int var_int64_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.i64 , 8) ;
    offset += 8 ;

    return offset ;
}

int var_uint64_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.u64 , 8) ;
    offset += 8 ;

    return offset ;
}

int var_float_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag)
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.f32 , 4) ;
    offset += 4 ;

    return offset ;
}

int var_double_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    ::memcpy(buffer + offset , &var->Data.f64 , 8) ;
    offset += 8 ;

    return offset ;
}

int var_string_encoder(const sdbus::Variant * var , char * buffer , size_t buflen , uint32_t tag , bool withtag)
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    const sdbus::string * str = NULL ;
    if(var->RefString(str) == false || str == NULL)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;


    int length = str->length() + 1;
    ::memcpy(buffer + offset , &length , 4) ;
    offset += 4 ;

    if(str->empty() == false)
    {
        ::memcpy(buffer + offset , str->c_str() , str->length()) ;
        offset += str->length();
    }

    buffer[offset++] = '\0' ;    

    return offset ;    
}

int var_raw_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    const sdbus::string * raw = NULL ;
    if(var->RefRaw(raw) == false || raw == NULL)
        return 0 ;

    int offset = 0 ;

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        ::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    ::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;


    int length = raw->length() ;
    ::memcpy(buffer + offset , &length , 4) ;
    offset += 4 ;

    if(raw->empty() == false)
    {
        ::memcpy(buffer + offset , raw->data() , length) ;
        offset += length ;
    }


    return offset ;    
}

int var_map_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    const VariantMap * map = NULL ;
    if(var->RefMap(map) == false || map == NULL)
        return 0 ;

    int offset = 0 ;
    

    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        //数据在后面保存
        //::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    //数据放在后面保存
    //::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    //预留size
    int osize = offset ;
    offset += 4 ;

    int cont_size = 0 ;
    int field_count = map->GetFieldCount() ;
    for(int fidx = 0 ; fidx < field_count ; ++ fidx)
    {
        const Variant * elem = NULL ;
        if(map->GetFieldValue(fidx , elem) == false || elem == NULL)
            continue ;

        int type = elem->Type ;
        if(type < Variant::VAR_MIN || type > Variant::VAR_MAX)
            continue ;

        variant_encoder_t encoder = ves[type] ;

        int csize = encoder(elem , buffer + offset , buflen - offset, 0 , true) ;
        offset += csize;
        cont_size += csize ;

        //LOGDEBUG("we have econde map  , index[%u] tag[%u] size[%u] total[%u]" , fidx , elem->Tag , csize , cont_size ) ;
    }

    if(cont_size < 0)
        return 0 ;

    int rsize = 0 ;
    if(withtag)
    {
        ::memcpy(buffer , &tag , 4) ;
        rsize += 4 ;
    }
    ::memcpy(buffer + rsize , &var->Type , 1) ;    
    ::memcpy(buffer + osize , &cont_size , 4) ;
    return offset ;
}

int var_list_encoder(const sdbus::Variant * var , char * buffer , size_t buflen , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag) == false)
        return 0 ;

    const VariantList * list = NULL ;
    if(var->RefList(list) == false || list == NULL)
        return 0 ;

    int offset = 0 ;   
    if(withtag)
    {
        if(tag == 0)
            tag = var->Tag ;

        //::memcpy(buffer , &tag , 4) ;
        offset += 4 ;
    }

    //::memcpy(buffer + offset , &var->Type , 1) ;
    offset += 1 ;

    //预留size
    int osize = offset ;
    offset += 4 ;

    int cont_size = 0 ;
    int count = list->Size() ;
    for(int idx = 0 ; idx < count ; ++idx)
    {
        const Variant* elem = list->Get(idx) ;
        if(elem == NULL)
            break ;

        int type = elem->Type ;
        if(type < Variant::VAR_MIN || type > Variant::VAR_MAX )
            continue ;

        variant_encoder_t encoder = ves[type] ;

        int csize = encoder(elem , buffer + offset , buflen - offset , 0 , false) ;
        offset += csize;
        cont_size += csize ;
    }

    if(cont_size < 0)
        return 0 ;

    int rsize = 0 ;
    if(withtag)
    {
        ::memcpy(buffer , &tag , 4) ;
        rsize += 4 ;
    }
    ::memcpy(buffer + rsize , &var->Type , 1) ;    
    ::memcpy(buffer + osize , &cont_size , 4) ;
    return offset ;
}

bool var_check_encode(const Variant * var , uint32_t tag , bool withtag) 
{
    if(var == NULL || var->Assigned() == false || var->Type == Variant::VAR_VOID)
        return false ;

    if(withtag == true)
    {
        if(tag == 0)
            tag = var->Tag  ;

        if(tag == 0)
            return false ;
    }

    return true ;
}

int var_void_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    return 0 ;
}

int var_bool_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag , withtag ) == false)
        return 0 ;

    int size = 1 + 1 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_int8_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 1 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_uint8_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 1 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_int16_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 2 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_uint16_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 2 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_int32_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 4 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_uint32_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 4 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_int64_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 8 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_uint64_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 8 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_float_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 4 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_double_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 + 8 ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_string_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;
    
    const sdbus::string * str = NULL ;
    if(var->RefString(str) == false || str == NULL)
        return 0 ;

    int size = 1 + 4 + (int)str->length() + 1;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_raw_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    
    const sdbus::string * raw = NULL ;
    if(var->RefRaw(raw) == false || raw == NULL)
        return 0 ;

    int size = 1 + 4 + (int)raw->length() ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_map_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 ;
    
    const VariantMap * map = NULL ;
    if(var->RefMap(map) == false || map == NULL)
        return 0 ;

    int cont_size = 0 ;
    int field_count = map->GetFieldCount() ;
    for(int fidx = 0 ; fidx < field_count ; ++ fidx)
    {
        const Variant * elem = NULL ;
        if(map->GetFieldValue(fidx , elem) == false || elem == NULL)
            continue ;

        int type = elem->Type ;
        if(type < Variant::VAR_MIN || type > Variant::VAR_MAX)
            continue ;

        variant_sizer_t sizer = vss[type] ;

        size_t fsize = sizer(elem , 0 , true) ;
        cont_size += fsize ;

        //LOGDEBUG("we have calc map size , index[%u] tag[%u] size[%u] total[%u]" , fidx , elem->Tag , fsize , cont_size ) ;

        //cont_size += sizer(elem , 0 , true) ;           
    }

    if(cont_size < 0)
        return 0 ;

    size += 4 + cont_size ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_list_sizer(const sdbus::Variant * var , uint32_t tag , bool withtag) 
{
    if(var_check_encode(var , tag  , withtag) == false)
        return 0 ;

    int size = 1 ;

    const VariantList * list = NULL ;
    if(var->RefList(list) == false || list == NULL)
        return 0 ;

    int cont_size = 0 ;
    int count = list->Size() ;
    for(int idx = 0 ; idx < count ; ++idx)
    {
        const Variant* elem = list->Get(idx) ;
        if(elem == NULL)
            break ;

        int type = elem->Type ;
        if(type < Variant::VAR_MIN || type > Variant::VAR_MAX)
            continue ;

        variant_sizer_t sizer = vss[type] ;

        cont_size += sizer(elem , tag , false) ;           
    }

    if(cont_size < 0)
        return 0 ;

    size += 4 + cont_size ;
    if(withtag)
        return 4 + size ;
    else
        return size ;
}

int var_void_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    return 0 ;
}
int var_bool_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.bv , buffer , 1) ;
    return 1 ;
}

int var_int8_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.i8 , buffer , 1) ;
    return 1 ;
}
int var_uint8_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.u8 , buffer , 1) ;
    return 1 ;
}

int var_int16_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.i16 , buffer , 2) ;
    return 2 ;
}
int var_uint16_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.u16 , buffer , 2) ;
    return 2 ;
}

int var_int32_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.i32 , buffer , 4) ;
    return 4 ;
}
int var_uint32_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.u32 , buffer , 4) ;
    return 4 ;
}

int var_int64_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.i64 , buffer , 8) ;
    return 8 ;
}
int var_uint64_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.u64 , buffer , 8) ;
    return 8 ;
}

int var_float_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.f32 , buffer , 4) ;
    return 4 ;
}
int var_double_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    var->Data.val = 0 ;
    memcpy(&var->Data.f64 , buffer , 8) ;
    return 8 ;
}

int var_string_decoder(sdbus::Variant * var , const char * buffer , size_t buflen) 
{
    int length = 0 , offset = 0;
    ::memcpy(&length , buffer , 4) ;
    offset += 4 ;

    sdbus::string * str = var->Data.str ;
    if(str == NULL)
    {
        str = new sdbus::string() ;
        var->Data.str = str ;
    }

    *str = buffer + offset ;    
    offset += length ;
    return offset ;
}

int var_raw_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    int length = 0 , offset = 0;
    ::memcpy(&length , buffer , 4) ;
    offset += 4 ;

    sdbus::string * raw = var->Data.raw ;
    if(raw == NULL)
    {
        raw = new sdbus::string() ;
        var->Data.raw = raw ;
    }

    raw->assign(buffer + offset , length) ;
    offset += length ;
    return offset ;
}

int var_map_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) 
{
    int length = 0 , offset = 0;
    ::memcpy(&length , buffer , 4) ;
    offset += 4 ;

    VariantMap * map = var->Data.map ;
    if(map == NULL)
    {
        map = new VariantMap() ;
        var->Data.map = map ;
    }

    const char * data = buffer + offset ;
    int clen = 0 ;
    while(clen < length)
    {
        uint32_t tag = 0 ;
        int type = 0 ;

        ::memcpy(&tag , data + clen , 4) ;
        clen += 4 ;

        ::memcpy(&type , data + clen , 1) ;
        clen += 1 ;

        if(type < Variant::VAR_MIN || type > Variant::VAR_MAX)
            break ;

        variant_decoder_t decoder = vds[type] ;
        //Variant * elem = map.AddVariant(tag) ;
        Variant * elem = map->AddVariant(tag) ;

        elem->Type = type ;

        int csize = decoder(elem , data + clen , length - clen) ;
        if(csize < 0)
            break ;

        elem->Flag = 1 ;

        clen += csize ;
    }

    offset += length ;
    return offset ;
}

int var_list_decoder(sdbus::Variant * var , const char * buffer , size_t buflen) 
{
    int length = 0 , offset = 0;
    ::memcpy(&length , buffer , 4) ;
    offset += 4 ;

    VariantList *list = var->Data.list ;
    if(list == NULL)
    {
        list = new VariantList(); 
        var->Data.list = list;
    }

    const char * data = buffer + offset ;
    int clen = 0 ;
    while(clen < length)
    {
        int type = 0 ;

        ::memcpy(&type , data + clen , 1) ;
        clen += 1 ;

        if(type < Variant::VAR_MIN || type > Variant::VAR_MAX)
            break ;

        variant_decoder_t decoder = vds[type] ;
        Variant * elem = list->AddVariant() ;
        elem->Type = type ;

        int csize = decoder(elem , data + clen , length - clen) ;
        if(csize < 0)
            break ;

        elem->Flag = 1 ;

        clen += csize ;
    }

    offset += length ;
    return offset ;
}


}

