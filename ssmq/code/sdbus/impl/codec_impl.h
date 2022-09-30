
#ifndef __SDBUS_CODEC_IMPL_H
#define __SDBUS_CODEC_IMPL_H 1


#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/variant.h"

namespace sdbus{
/*
    2014-09-18
    如果某个tag的值为空，原来是会被忽略。但是业务仍然需要
    传递这个tag。以区别和没有的情况。
*/

bool var_check_encode(const Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_void_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_bool_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;

int var_int8_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_uint8_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;

int var_int16_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_uint16_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;

int var_int32_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_uint32_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;

int var_int64_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_uint64_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;

int var_float_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_double_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;

int var_string_encoder(const sdbus::Variant * var , char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_raw_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;

int var_map_encoder(const sdbus::Variant * var ,  char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;
int var_list_encoder(const sdbus::Variant * var , char * buffer , size_t buflen , uint32_t tag = 0 , bool withtag = true) ;



int var_void_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_bool_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_int8_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_uint8_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_int16_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_uint16_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_int32_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_uint32_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_int64_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_uint64_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_float_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_double_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_string_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_raw_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;

int var_map_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;
int var_list_sizer(const sdbus::Variant * var , uint32_t tag = 0 , bool withtag = true) ;


int var_void_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;
int var_bool_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;

int var_int8_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;
int var_uint8_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;

int var_int16_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;
int var_uint16_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;

int var_int32_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;
int var_uint32_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;

int var_int64_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;
int var_uint64_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;

int var_float_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;
int var_double_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;

int var_string_decoder(sdbus::Variant * var , const char * buffer , size_t buflen) ;
int var_raw_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;

int var_map_decoder(sdbus::Variant * var ,  const char * buffer , size_t buflen) ;
int var_list_decoder(sdbus::Variant * var , const char * buffer , size_t buflen) ;

}

#endif  /** __SDBUS_CODEC_IMPL_H */
