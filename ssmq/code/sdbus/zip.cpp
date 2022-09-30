
#include "sdbus/compile.h"
#include "sdbus/zip.h"
#include "SSRTL/SSZip.h"

namespace sdbus{

bool zip(const char * ibuf , int ilen , char * obuf , int& olen)
{
    return ss::zip(ibuf , ilen , obuf , olen) ;
}

bool unzip(const char * ibuf , int ilen , char * obuf , int& olen)
{
    return ss::unzip(ibuf , ilen , obuf , olen) ;
}

}