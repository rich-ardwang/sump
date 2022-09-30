
#include "SSZip.h"
#include "zlib/zlib.h"

namespace ss {


	bool zip(const char * ibuf , int ilen , char * obuf , int& olen)
	{
		z_stream  strm ;
		int ret ;

		strm.zalloc = Z_NULL ;
		strm.zfree = Z_NULL ;
		strm.opaque = Z_NULL ;

		try
		{
			ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
			if (ret != Z_OK)
			{
				return false;
			}
			strm.avail_in = ilen;
			strm.next_in = (Bytef *)ibuf ;
			strm.avail_out = olen ;
			strm.next_out = (Bytef *)obuf;
			ret = deflate(&strm, Z_FINISH);
			if (ret != Z_STREAM_ERROR)
			{
				olen = olen - strm.avail_out;
				deflateEnd(&strm);
				return true;
			}
			else
			{
				deflateEnd(&strm);
				return false;
			}
		}
		catch(...)
		{
			deflateEnd(&strm);
			return false;
		}
	
	}

	bool unzip(const char * ibuf , int ilen , char * obuf , int& olen) 
	{
		int ret;
		unsigned have;
		z_stream strm;

		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;

		ret = inflateInit(&strm);
		if(ret != Z_OK)
			return false ;

		strm.avail_in = ilen ;
		strm.next_in = (Bytef *)ibuf ;

        strm.avail_out = olen;
        strm.next_out = (Bytef *)obuf;
        ret = inflate(&strm, Z_NO_FLUSH);

        if(ret == Z_STREAM_ERROR)
		{
			return false ;
		}

        switch (ret) 
		{
		case Z_NEED_DICT:
			ret = Z_DATA_ERROR;     /* and fall through */
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			(void)inflateEnd(&strm);
			return false;
        }

        have = olen - strm.avail_out;
		olen = have ;

		(void)inflateEnd(&strm);
		return (ret == Z_STREAM_END) ? true : false;
	}

}
