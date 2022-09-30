
#include "SSRTL/SSLog.h"
#include "SSRTL/SSThread.h"
#include <stdarg.h>
#include "SSRTL/SSString.h"
#include <boost/filesystem.hpp> 
#include <boost/date_time.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  


namespace bf = boost::filesystem;
namespace bd = boost::gregorian;

namespace ss{

	void logger::init()
	{
		file_ = NULL ;
        //level_ = LOG_LEVEL::LOG_DEBUG ;
        level_ = LOG_LEVEL::LOG_ERR ;
		tty_level_ = LOG_LEVEL::LOG_EMERG ;

		name_ = "LOG" ;

		max_size_ = 0x70123456 ;
	}

	logger::logger()
	{
		init() ;
		open() ;
	}

	logger::logger(const char *name)
	{
		init() ;
		name_ = name ;

		open() ;
	}

	logger::~logger()
	{
		close() ;
	}

	void logger::open()
	{
		char file_name[256] ;

		today_.calc_now() ;

		sprintf(file_name , "%s%04d%02d%02d.log" ,name_.c_str() , today_.year() , today_.month() , today_.day()) ;

		unique_lock<shared_mutex> lock(guard_) ;
		if(file_ != NULL)
			return ;

		file_name_ = file_name ;
        try
        {
            bf::path curfile(file_name);
            if (!bf::exists(curfile))
            {
                clear_expired();
            }
        }
        catch (bf::filesystem_error e)
        {
            boost::system::error_code ec = e.code();
        }

		file_ = fopen(file_name , "a+b") ;	
	}

	void logger::close()
	{
		unique_lock<shared_mutex> lock(guard_) ; 
		if(file_)
		{
			fflush(file_) ;
			fclose(file_) ;

			file_ = NULL ;
		}
	}

    void logger::clear_expired() 
    {
        bf::path curfile(file_name_);
        bf::path curdir = curfile.branch_path();

        bf::directory_iterator iter = bf::directory_iterator(curdir);
        while (iter != bf::directory_iterator())
        {
            std::string sfile = iter->path().leaf().string();
            if (is_expired(sfile))
            {
                system::error_code ec;
                bf::remove(iter->path(), ec);
            }
            ++iter;
        }
    }

    bool logger::is_expired(const std::string& sfile)
    {
        bf::path raw_path(name_) ;
        std::string raw_name = raw_path.leaf().string();
        size_t l = raw_name.length();
        if (sfile.substr(0, l) != raw_name)
        {
            return false;
        }
        if (sfile.length() != l + 12)
        {
            if (sfile.substr(sfile.length() - 4, 4) == ".log")
            {
                return true;
            }
            return false;
        }
        int y = atoi(sfile.substr(l, 4).c_str());
        int m = atoi(sfile.substr(l + 4, 2).c_str());
        int d = atoi(sfile.substr(l + 6, 2).c_str());

        bd::date checkdate(y, m, d);
        bd::date curdate(today_.year(), today_.month(), today_.day());

        int nexpired = (curdate - checkdate).days();
        if (nexpired > 15)
        {
            return true;
        }
        
        return false;
    }

	int logger::set_level(int level)
	{
		level_ = level ;
		return level_ ;
	}

    int logger::set_level_by_config(const char* cfg_path, const char* node_name) 
    {
        if (!bf::exists(cfg_path))
        {
            return set_level(LOG_LEVEL::LOG_ERR);
        }

        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini(cfg_path, pt);
        return set_level(pt.get<int>(node_name, LOG_LEVEL::LOG_ERR));
    }

	int logger::set_stdout_level(int level)
	{
		return set_tty_level(level) ;
	}

	int logger::set_tty_level(int level)
	{
		tty_level_ = level ;
		return tty_level_ ;	
	}

	int logger::get_level() const
	{
		return level_ ;
	}

    int logger::get_tty_level() const
    {
        return tty_level_ ;
    }

	int logger::set_max_size(size_t max_size)
	{
		max_size_ = max_size ;
		return max_size_ ;	
	}

	const char *trim_file_name(const char *file_name)
	{
		const char *pstr ;

		if(file_name == NULL)
			return NULL ;

		pstr = file_name + strlen(file_name) ;
		while(pstr != file_name)
		{
			if(*pstr == '/' || *pstr == '\\')
			{
				pstr++ ;
				break ;
			}
			pstr-- ;
		}

		return pstr ;
	}

	const char *log_level[8] =
	{
		"emerg",
		"alert",
		"crit",
		"error",
		"warn",
		"notic",
		"info",
		"debug"
	};

	void logger::write_message(const char * message)
	{
		shared_lock<shared_mutex> lock(guard_) ;
		if(file_ == NULL || message == NULL)
			return ;

		int fsize = fprintf(file_ ,"%s \n", message);
		if(fsize > 0)
		{
			fflush(file_);
		}
	}

	int logger::write(const char *file_name , int line , int level , const char *format , ...)
	{
		va_list ap;
		char message[MAX_LOG_LEN] = {'\0'} , *pos ;
		int len = 0 ;

		if(level > level_)
			return 0 ;

		//实现日志按每日存放的功能！
		if(today_ != date::today())
		{
			close() ;
			open() ;
		}

		utime now(true) ;
		if(level >= 0 && level <= LOG_LEVEL::LOG_LEVEL_MAX)
		{
			 len = sprintf(message , "%s [%s] TID[%u] FILE[%s]LINE[%d] " , 
				 now.to_string().c_str() , log_level[level] , current_thread_id() ,
				 trim_file_name(file_name) , line);
		}
		else
		{
			 len = sprintf(message , "%s TID[%u] FILE[%s]LINE[%d] " , 
				 now.to_string().c_str() ,current_thread_id() , 
				 trim_file_name(file_name) , line);
		}

		pos = message + len ;
		len = MAX_LOG_LEN - len - 30;

		va_start(ap, format);
		vsnprintf(pos, len , format, ap);
		va_end(ap);

		write_message(message) ;

		if(tty_level_ >= level)
		{
			printf("%s \n", message) ;
		}

		return 0 ;		
	}

	int logger::writebuf(const char *file_name , int line , int level , const void *buf , int size)
	{
		char message[MAX_LOG_LEN] = {'\0'} , *pos ;
		int len = 0 ;

		if(level > level_)
			return 0 ;

		//实现日志按每日存放的功能！
		if(today_ != date::today())
		{
			close() ;
			open() ;
		}

		utime now(true) ;
		if(level >= 0 && level <= LOG_LEVEL::LOG_LEVEL_MAX)
		{
			 len = sprintf(message , "%s [%s] TID[%u] FILE[%s]LINE[%d] " , 
				 now.to_string().c_str() , log_level[level] , current_thread_id() , 
				 trim_file_name(file_name) , line);
		}
		else
		{
			 len = sprintf(message , "%s TID[%u] FILE[%s]LINE[%d] " , 
				 now.to_string().c_str() ,current_thread_id() , trim_file_name(file_name) , line);
		}

		pos = message + len ;
		len = MAX_LOG_LEN - len ;

		const unsigned char * pbuf = (const unsigned char *)buf ;
		int i , mlen = 0 , lineno = 0 ;
		int last_line_pos = 0 , tlen = 0;
		int fsize = 0 ;
		/*
			2011-07-01
			必须限制最大缓冲区大小，为1M
		*/
		for(i = 0 ; i < size && i < 0x100000 ; i++)
		{
			if((i % 32) == 0 || mlen == 0)
			{
				if((i % 1024) == 0)
				{
					//每32行，共1024个字节，打印一段
					tlen = sprintf(pos + mlen , "[%d / %d] \n" , i , size) ;
					mlen += tlen ;
				}
				sprintf(pos + mlen , "\t[%03d] " , lineno) ;
				mlen += 7 ;
			}

			//sprintf(pos + mlen , " %02hhX" , pbuf[i]) ;
			hex_sprintf(pos + mlen , pbuf[i]) ;
			mlen += 2 ;
			pos[mlen++] = ' ' ;

			if((i + 1) % 32 == 0)
			{
				last_line_pos = mlen ;
				pos[mlen++] = '\n' ;
				lineno++ ;
			}

			if(((i + 1) % 1024) == 0 || mlen >= (len - 10))
			{
				//缓冲区不足，直接打印到最后一行
				pos[last_line_pos] = '\0' ;

				/*
				fsize = fprintf(file_ ,"%s \n", message);
				if(fsize > 0)
				{
					fflush(file_);
				}
				*/
				write_message(message) ;

				if(tty_level_ >= level)
				{
					printf("%s \n", message) ;
				}

				mlen -= last_line_pos + 1 ;
				if(mlen > 0)
				{
					memcpy(pos , pos + last_line_pos + 1 , mlen) ;
				}
				last_line_pos = 0 ;
			}
		}

		if(mlen > 0)
		{
			pos[mlen] = '\0' ;

			/*
			fsize = fprintf(file_ ,"%s \n", message);
			if(fsize > 0)
			{
				fflush(file_);
			}
			*/
			write_message(message) ;

			if(tty_level_ >= level)
			{
				printf("%s \n", message) ;
			}
		}

		if(i >= 0x100000)
		{
			sprintf(pos , "buf size [%d] , overflow 1M" , size) ;
			/*
			fprintf(file_ , "%s \n" , message) ;
			fflush(file_) ;
			*/
			write_message(message) ;
		}

		return 0 ;			
	}

}

ss::logger *default_logger = NULL;

ss::logger * get_logger() 
{
	return default_logger ;
}

class default_logger_auto_singleton {
public:
	default_logger_auto_singleton()
	{
		if(default_logger == NULL)
		{
			default_logger = new ss::logger("log\\") ;
		}
	}

	~default_logger_auto_singleton()
	{
		if(default_logger != NULL)
		{
			delete default_logger ;
			default_logger = NULL ;
		}
	}
} ;

default_logger_auto_singleton default_logger_singleton ;
