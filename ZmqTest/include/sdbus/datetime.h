
#ifndef __SDBUSAPI_DATETIME_H
#define __SDBUSAPI_DATETIME_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include <time.h>

namespace sdbus{

/*
	������ͬһ�죬����û�б�Ҫ��ʱ���б������ڵ���Ϣ�������ڵ����Ǹ����ǵĴ���
	����ʱ����˵��ÿ��3600*24 = 86400�룬�Ǹ�������

	���������գ����Ǳ�ʾ����ʱ�䡣

	����time.h�м�����Ҫ�������͡�
	time(NULL) �����ص�ֵ��ͬ����ʱ�䵽EPOCH��������ͬ
	mktime(struct tm)��Ҳ�ǻ��ڱ���ʱ����м���
	�����ر�ָ���������ʱ�����������ǻ��ڱ���ʱ��

    32λ��linux��time_t��32λ��windows��64λlinux����64λ����ע����32λlinux�µ�������⡣    
*/
class SDBUSAPI Date{
public:
    Date() ;
    Date(const Date& date) ;
    Date(time_t ts) ;
    Date(int year , int month , int day) ;
    void Reset() ;

    Date& operator=(time_t ts) ;
    bool operator==(time_t ts) const;
    bool operator!=(time_t ts) const;
    bool operator<(time_t ts) const;
    bool operator<=(time_t ts) const;
    bool operator>(time_t ts) const;
    bool operator>=(time_t ts) const;
    
    Date& operator=(const Date& date) ;
	bool operator==(const Date& date) const;
	bool operator!=(const Date& date) const;
	bool operator<(const Date& date) const;
	bool operator<=(const Date& date) const;
	bool operator>(const Date& date) const;
	bool operator>=(const Date& date) const;


    const sdbus::string& ToString(char separator = 0) const ;
    time_t Time() const;
    void Time(time_t ts) ;
    time_t GMTime() const ;

    inline int Year() const {return year_ ;}
    inline int Month() const {return month_ ;}
    inline int Day() const {return day_ ;}
    inline int WDay() const {return wday_ ;}

    Date& NextDay(int dc = 1) ;

    void CalcNow() ;

    static const Date& Today() ;
    static int TimeZone() ;

private:
    time_t time_ ;
    int year_ ;     //�� 2014
    int month_ ;    //�� 1...12
    int day_ ;      //����1...31
    int wday_ ;     //�� 1..7

    mutable sdbus::string datestr_ ;
    mutable int stage_ ;

    int Compare(const Date& date) const ;
    int Compare(time_t t) const ;

    void Assign(const Date& date) ;
    void Assign(time_t t) ;
} ;

class SDBUSAPI STime {
public:
    STime() ;
    STime(time_t t) ;
    STime(const STime& st) ;

    ~STime() ;

    STime& operator=(time_t t) ;
    bool operator==(time_t t) const ;
    bool operator!=(time_t t) const ;
    bool operator <(time_t t) const ;
    bool operator<=(time_t t) const ;
    bool operator >(time_t t) const ;
    bool operator>=(time_t t) const ;

    STime& operator=(const STime& st) ;
    bool operator==(const STime& st) const ;
    bool operator!=(const STime& st) const ;
    bool operator <(const STime& st) const ;
    bool operator<=(const STime& st) const ;
    bool operator >(const STime& st) const ;
    bool operator>=(const STime& st) const ;

    void Reset() ;
    void CalcNow() ;

    inline time_t Time() const {return time_ ;}
    inline int Hour() const {return hour_ ;}
    inline int Minute() const {return minute_ ;}
    inline int Second() const {return second_ ;}

    const sdbus::string& ToString() const ;

    /*
        ���ؾ��뵱��0�������
    */
    static time_t Now() ;
private:
    time_t time_ ;
    int hour_ ;
    int minute_ ;
    int second_ ;

    mutable sdbus::string timestr_ ;
    mutable int stage_ ;

    int Compare(time_t t) const ;
    int Compare(const STime& st) const ;
    void Assign(time_t t) ;
    void Assign(const STime& st) ;
} ;

SDBUSAPI bool LocalTime(struct tm * tm , time_t t) ;
SDBUSAPI bool GMTime(struct tm * tm , time_t t) ;
SDBUSAPI time_t MakeTime(const struct tm * tm) ;

/*
    ��ʱ������tת���ɱ���ʱ�䣬�������ʱ��������
*/
SDBUSAPI bool DateStartTime(time_t t , time_t& ts , int& year , int& month , int& day , int& wday) ;
SDBUSAPI bool DateStartTime(time_t t , time_t& ts) ;

/*
    ����ӵ���0�㿪ʼ����ʱ�̵�����
*/
SDBUSAPI bool DateLeftTime(time_t t , time_t& left) ;

/*
    ��������ʱ�䡣֧�ָ�ʽΪ"YYYY-MM-DD HH:MM:SS.mmm"����"YYYYMMDD HH:MM:SS.mmm" ����"YYYY/MM/DD HH:MM:SS.mmm"
    ��������зָ�������ô���Բ�����0
*/
SDBUSAPI bool ParseTimeStr(const char * str , int&year , int& month , int& day , int& hour , int& minute , int& second , int& msec) ;

SDBUSAPI time_t MakeTime(int year , int month , int day , int hour = 0, int minute = 0, int second = 0) ;


}

#endif  /** __SDBUSAPI_DATETIME_H */
