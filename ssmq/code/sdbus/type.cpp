
#include "sdbus/type.h"

namespace sdbus{

int64_t atoi64(const char * str)
{
    if(str == NULL)
        return 0 ;

#if (OSTYPE == OSTYPE_WINDOWS)
    return ::_atoi64(str) ;
#else
#if(WORDSIZE == 32)
    return ::atoll(str) ;
#else
    return ::atol(str) ;
#endif    
#endif
}

}
