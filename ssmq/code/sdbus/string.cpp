
#include "sdbus/string.h"

namespace sdbus{

static const char __k_empty_string__[1] = {'\0'} ;
const char * string::kEmptyString = __k_empty_string__ ;

const string string::EMPTY ;

/*
    保证最后有个地方可以写入\0
*/
size_t align_size(size_t size)
{
    size_t tmp = size + 1 ;
    size_t mode = tmp >> 4 ;
    size_t left = ((tmp & 15) == 0) ? 0 : 1 ;

    return ((mode + left) << 4) ;
}

void string::init() 
{
    buffer_ = NULL ;
    length_ = 0 ;
    capacity_ = 0 ;
}
string::string()
{
    init() ;
}

string::~string()
{
    if(buffer_ != NULL)
        ::free(buffer_) ;
    init() ;
}

string::string(const string& str)
{
    init() ;
    assign(str.c_str() , str.length()) ;
}

string::string(const char * str)
{
    init() ;
    if(str == NULL)
        return ;

    size_t len = ::strlen(str) ;
    assign(str , len) ;
}

string::string(const char * str , size_t size)
{
    init() ;
    assign(str , size) ;
}

string& string::operator= (const string& str)
{
    assign(str.c_str() , str.length()) ;
    return (*this) ;
}

string& string::operator= (const char * str)
{
    size_t len = ::strlen(str) ;
    assign(str , len) ;
    return (*this) ;
}

void string::clear()
{
    length_ = 0 ;
}

void string::assign(const char * str , size_t size)
{
    size_t buflen = align_size(size) ;
    length_ = 0 ;
    if(capacity_ < buflen)
    {
        capacity_ = 0 ;
        if(buffer_ != NULL)
            ::free(buffer_) ;

        buffer_ = (char *)::malloc(buflen) ;
        if(buffer_ == NULL)
            return ;

        capacity_ = buflen ;
    }

    if(str != NULL)
        ::memcpy(buffer_ , str , size) ;

    buffer_[size] = '\0' ;
    length_ = size ;
}
int string::cmp(const char * str , size_t size) const
{
    if(buffer_ == NULL)
    {
        if(str == NULL)
            return 0 ;
        else
            return -1 ;
    }
    else
    {
        if(str == NULL)
            return 1 ;
    }

    if(size == 0)
    {
        size = ::strlen(str) ;
    }

    size_t len = (size <= length_) ? size : length_ ;
    int ret = ::memcmp(buffer_ , str , size) ;
    if(ret == 0)
        ret = (int)(length_ - size) ;
    return ret ;
}

string& string::append(const char * str) 
{
    size_t len = ::strlen(str) ;
    if(len + length_ >= capacity_)
    {
        char * tmp = buffer_ ;

        size_t size = align_size(len + length_) ;
        buffer_ = (char *)::malloc(size) ;
        if(buffer_ == NULL)
        {
            buffer_ = tmp ;
            return (*this);
        }
        if(tmp != NULL && length_ > 0)
        {
            ::memcpy(buffer_ , tmp , length_) ;
            ::free(tmp) ;
        }

        capacity_ = size ;
    }

    if(str != NULL && len > 0)
    {
        ::memcpy(buffer_ + length_ , str , len) ;
        length_ += len ;
        buffer_[length_] = '\0' ;
    }

    return (*this) ;
}

void string::attach(string& str) 
{
    buffer_ = str.buffer_ ;
    length_ = str.length_ ;
    capacity_ = str.capacity_ ;

    str.init() ;
}


int string::trim() 
{
    if(length_ == 0  || buffer_ == NULL)
        return 0 ;

    int offset1 , offset2 ;
    for(offset1 = 0 ; offset1 < (int)length_ ; ++offset1)
    {
        if(buffer_[offset1] == ' ')
            continue ;
        break ;
    }

    for(offset2 = length_ - 1 ; offset2 >= 0 ; --offset2)
    {
        if(buffer_[offset2] == ' ')
            continue ;
        break ;
    }

    int len = offset2 - offset1 ;
    if(len < 0)
        len = 0 ;
    else
        ++len ;

    if(len >= (int)length_)
        return len ;

    if(offset1 != 0 && len > 0)
        ::memmove(buffer_ , buffer_ + offset1 , len) ;

    buffer_[len] = '\0' ;
    length_ = len ;

    return len ;
}

static const char upper_2_lower = 'Z' - 'z' ;

void string::to_upper()
{
    for(size_t idx = 0 ; idx < length_ ; ++idx)
    {
        char& ch = buffer_[idx] ;
        if(ch >= 'a' && ch <= 'z')
        {
            buffer_[idx] = upper_2_lower + ch ;
        }
    }
}

void string::to_lower()
{
    for(size_t idx = 0 ; idx < length_ ; ++idx)
    {
        char& ch = buffer_[idx] ;
        if(ch >= 'A' && ch <= 'Z')
        {
            buffer_[idx] = ch - upper_2_lower ;
        }
    }
}

const string string::upper() const
{
    string ustr(*this) ;
    ustr.to_upper() ;
    return ustr ;
}

const string string::lower() const
{
    string lstr(*this) ;
    lstr.to_lower() ;
    return lstr ;
}

}

