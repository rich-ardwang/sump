
#include "sdbus/datetime.h"

namespace sdbus{

Date::Date()
{
    Reset() ;
    CalcNow() ;
}

Date::Date(const Date& date)
{
    Reset() ;
    Assign(date) ;
}

Date::Date(time_t ts)
{
    Reset() ;
    Assign(ts) ;
}

Date::Date(int year , int month , int day)
{
    Reset() ;
    time_t t = MakeTime(year , month , day) ;
    DateStartTime(t , time_ , year_ , month_ , day_ , wday_) ;
}

void Date::Reset()
{
    time_ = 0 ;
    year_ = 0 ;
    month_ = 0 ;
    day_ = 0 ;
    wday_ = 0 ;
    datestr_.clear() ;
    stage_ = 0 ;
}

Date& Date::operator=(time_t ts)
{
    Assign(ts) ;
    return (*this) ;
}

bool Date::operator==(time_t ts) const
{
    return Compare(ts) == 0 ;
}

bool Date::operator!=(time_t ts) const
{
    return Compare(ts) != 0 ;
}

bool Date::operator<(time_t ts) const
{
    return Compare(ts) < 0 ;
}

bool Date::operator<=(time_t ts) const
{
    return Compare(ts) <= 0 ;
}

bool Date::operator>(time_t ts) const
{
    return Compare(ts) > 0 ;
}

bool Date::operator>=(time_t ts) const
{
    return Compare(ts) >= 0 ;
}
    

bool Date::operator==(const Date& date) const
{
    return Compare(date) == 0 ;
}

bool Date::operator!=(const Date& date) const
{
    return Compare(date) != 0 ;
}

bool Date::operator<(const Date& date) const
{
    return Compare(date) < 0 ;
}

bool Date::operator<=(const Date& date) const
{
    return Compare(date) <= 0 ;
}

bool Date::operator>(const Date& date) const
{
    return Compare(date) > 0 ;
}

bool Date::operator>=(const Date& date) const
{
    return Compare(date) >= 0 ;
}

const sdbus::string& Date::ToString(char separator) const 
{
    if(stage_ < 2)
    {
        char str[256] = {'\0'} ;
        if(separator == '\0')
            ::sprintf(str , "%04d%02d%02d" , year_ , month_ , day_) ;
        else
            ::sprintf(str , "%04d%hhc%02d%hhc%02d" , year_ , separator , month_ , separator , day_) ;
        datestr_ = str ;
        stage_ = 2 ;
    }

    return datestr_ ;
}

time_t Date::Time() const
{
    return time_ ;
}

void Date::Time(time_t ts) 
{
    Assign(ts) ;
}

time_t Date::GMTime() const 
{
    return (time_ - Date::TimeZone() * 3600) ;
}

Date& Date::NextDay(int dc) 
{
    time_t ts = time_ + dc * 86400 ;
    Reset() ;

    Time(ts) ;

    return (*this) ;
}

void Date::CalcNow() 
{
    Reset() ;
    if(DateStartTime(::time(NULL) , time_ , year_ , month_ , day_ , wday_) == true)
        stage_ = 1 ;
}

const Date& Date::Today()
{
    static Date __today__ ;
    static time_t last_time = __today__.Time() ;

    time_t now = ::time(NULL) ;
    if(now - last_time >= 86400)
    {
        __today__.CalcNow() ;
        last_time = __today__.Time() ;
    }

    return __today__ ;
}

int Date::TimeZone() 
{
    static int __local_timezone__ = -1024 ;

    if(__local_timezone__ == -1024)
    {
        time_t now = ::time(NULL) ;

        struct tm ltm , gtm ;

        sdbus::LocalTime(&ltm , now) ;
        sdbus::GMTime(&gtm , now) ;

        __local_timezone__ = (ltm.tm_hour + 24 - gtm.tm_hour) % 24 ;
    }

    return __local_timezone__ ;
}

int Date::Compare(const Date& date) const
{
    return (int)(time_ - date.time_) ;
}

int Date::Compare(time_t t) const 
{
    time_t ts ;
    DateStartTime(t , ts) ;
    return (int)(time_ - ts) ;
}

void Date::Assign(const Date& date)
{
    time_ = date.time_ ;
    year_ = date.year_ ;
    month_ = date.month_ ;
    day_ = date.day_ ;
    wday_ = date.wday_ ;
    datestr_.clear() ;
    stage_ = 1 ;
}

void Date::Assign(time_t t)
{
    if(stage_ != 0)
        Reset() ;
    DateStartTime(t , time_ , year_ , month_ , day_ , wday_) ;
    stage_ = 1 ;
}

#if (OSTYPE == OSTYPE_WINDOWS)
bool LocalTime(struct tm * tm , time_t t)
{
    return (::localtime_s(tm , &t) == 0) ;
}

bool GMTime(struct tm * tm , time_t t)
{
    return (::gmtime_s(tm , &t) == 0) ;
}
#else
bool LocalTime(struct tm * tm , time_t t)
{
    return (::localtime_r(&t , tm) == 0) ;
}

bool GMTime(struct tm * tm , time_t t)
{
    return (::gmtime_r(&t ,  tm) == 0) ;
}
#endif


time_t MakeTime(const struct tm * tm)
{
    return ::mktime((struct tm *)tm) ;
}

bool DateStartTime(time_t t , time_t& ts ,int& year , int& month , int& day , int& wday) 
{
    struct  tm tm ;
    if(LocalTime(&tm , t) == false)
        return false ;

    year = tm.tm_year + 1900 ;
    month = tm.tm_mon + 1 ;
    day = tm.tm_mday ;
    wday = tm.tm_wday ;
    if(wday == 0)
        wday = 7 ;

    tm.tm_hour = 0;
    tm.tm_min = 0 ;
    tm.tm_sec = 0 ;

    ts = MakeTime(&tm) ;
    return true ;
}

bool DateStartTime(time_t t , time_t& ts) 
{
    int year , month , day , wday ;
    return DateStartTime(t , ts , year , month , day , wday) ;
}

bool DateLeftTime(time_t t , time_t& left) 
{
    time_t dst = 0 ;
    if(DateStartTime(t , dst) == false)
        return false ;

    left = t - dst ;
    return true ;
}


bool ParseTimeStr(const char * str , int&year , int& month , int& day , int& hour , int& minute , int& second , int& msec)
{
	year = month = day = hour = minute = second = msec = 0 ;
	if(str == NULL)
		return false ;

	const char * pchar = str ;
	int stage = 0 ;
	char ch = 0 , buffer[32] = {'\0'};
	int buflen = 0 ;
	do{
		ch = pchar[0] ;

		if(stage >= 0 && stage <= 2)
		{
			//日期，'-','/'以及最后的空格作为分隔符，如果没有分隔符，按长度分隔
			bool finish = false , seperate = false;
			if(ch >= '0' && ch <= '9')
			{
				buffer[buflen++] = ch ;
			}
			else
			{
				seperate = finish = true ;		//有分隔符，那么同时表示结束
			}

			if(finish == false)
			{
				if(stage == 0)
				{					
					if(buflen >= 5) 
					{
						finish = true ; 
						buflen = 4 ;
					}
				}
				else if(stage == 1 || stage == 2)
				{
					if(buflen >= 3) 
					{
						finish = true ; 
						buflen = 2 ;
					}
				}				
			}

			if(finish == true)
			{
				buffer[buflen] = '\0' ;
				switch(stage)
				{
				case 0 : year = atoi(buffer) ; break ;
				case 1 : month = atoi(buffer) ;  break ;
				case 2 : day = atoi(buffer) ; break ;
				default :break ;
				}

				buflen = 0 ;
				if(seperate == false)
				{
					buffer[buflen++] = ch ;
				}
				stage++ ;				
			}

		}
		else if(stage >= 3 && stage <= 6)
		{
			//时间，必须以":"分隔，中间可能没有0
			if(ch >= '0' && ch <= '9')
			{
				buffer[buflen++] = ch ;
			}
			else if(ch == ':' || ch == '.' || ch == '\0')
			{
				buffer[buflen] = '\0' ;
				switch(stage)
				{
				case 3 : hour = atoi(buffer) ; break ;
				case 4 : minute = atoi(buffer) ; break ;
				case 5 : second  = atoi(buffer) ; break ;
				case 6 : msec = atoi(buffer) ; break ;
				default : break ;
				}
				
				buflen = 0 ;
				stage++ ;
			}
		}

		pchar ++ ;	
		
	} while(ch != 0) ;

	return true ;	
}

time_t MakeTime(int year , int month , int day , int hour , int minute , int second) 
{
	struct tm tm ;
	memset(&tm , 0 , sizeof(tm)) ;

	tm.tm_year = year - 1900 ;
	tm.tm_mon = month - 1 ;
	tm.tm_mday = day ;

	tm.tm_hour = hour ;
	tm.tm_min = minute ;
	tm.tm_sec = second ;	

	return ::mktime(&tm) ;
}

STime::STime()
{
    Reset() ;
    CalcNow() ;
}

STime::STime(time_t t)
{
    Reset() ;
    Assign(t) ;
}

STime::STime(const STime& st)
{
    Reset() ;
    Assign(st) ;
}

STime::~STime()
{
    //
}

STime& STime::operator=(time_t t)
{
    Assign(t) ;
    return (*this) ;
}

bool STime::operator==(time_t t) const
{
    return Compare(t) == 0 ;
}

bool STime::operator!=(time_t t) const
{
    return Compare(t) != 0 ;
}

bool STime::operator <(time_t t) const
{
    return Compare(t) < 0 ;
}

bool STime::operator<=(time_t t) const
{
    return Compare(t) <= 0 ;
}

bool STime::operator >(time_t t) const
{
    return Compare(t) > 0 ;
}

bool STime::operator>=(time_t t) const
{
    return Compare(t) >= 0 ;
}

STime& STime::operator=(const STime& st)
{
    Assign(st) ;
    return (*this) ;
}

bool STime::operator==(const STime& st) const
{
    return Compare(st) == 0 ;
}

bool STime::operator!=(const STime& st) const
{
    return Compare(st) != 0 ;
}

bool STime::operator <(const STime& st) const
{
    return Compare(st) < 0 ;
}

bool STime::operator<=(const STime& st) const
{
    return Compare(st) <= 0 ;
}

bool STime::operator >(const STime& st) const
{
    return Compare(st) > 0 ;
}

bool STime::operator>=(const STime& st) const
{
    return Compare(st) >= 0 ;
}

void STime::Reset()
{
    time_ = 0 ;
    hour_ = 0 ;
    minute_ = 0 ;
    second_ = 0 ;
    timestr_.clear() ;
    stage_ = 0 ;
}

void STime::CalcNow() 
{
    time_t now = ::time(NULL) ;
    Assign(now) ;
}

const sdbus::string& STime::ToString() const
{
    if(stage_ < 2)
    {
        char str[256] = {'\0'} ;
        ::sprintf(str , "%02d:%02d:%02d" , hour_ , minute_ , second_) ;
        timestr_ = str ;
        stage_ = 2 ;
    }

    return timestr_ ;
}

time_t STime::Now()
{
    time_t t = ::time(NULL) ;
    time_t left = 0 ;
    DateLeftTime(t , left) ;
    return left ;
}

int STime::Compare(time_t t) const
{
    time_t left ;
    DateLeftTime(t , left) ;
    return (int)(time_ - left) ;
}

int STime::Compare(const STime& st) const
{
    return (int)(time_ - st.time_) ;
}

void STime::Assign(time_t t)
{
    if(stage_ != 0)
        Reset() ;

    DateLeftTime(t , time_) ;

    hour_ = (int)(time_ / 3600) ;
    minute_ = (int)((time_ % 3600) / 60) ;
    second_ = (int)(time_ % 60) ;

    stage_ = 1 ;
}

void STime::Assign(const STime& st)
{
    time_ = st.time_ ;
    hour_ = st.hour_ ;
    minute_ = st.minute_ ;
    second_ = st.second_ ;

    timestr_.clear() ;

    stage_ = 1 ;
}

}

