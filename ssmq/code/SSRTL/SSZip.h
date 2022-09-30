
#ifndef __SS_ZIP_H
#define __SS_ZIP_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace ss{


	bool zip(const char * ibuf , int ilen , char * obuf , int& olen) ;

	bool unzip(const char * ibuf , int ilen , char * obuf , int& olen) ;

}

#endif /** __SS_ZIP_H */
