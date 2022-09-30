/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName	:  helper.h
  *Author	:  scofined.qi
  *Version	:  1.0
  *Date		:  2017/10/27
  *Desc		:  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others	:  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History	:  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#ifndef HELPER_H_5BAA5D36_47EE_4D3E_BD24_E3BF13A30200
#define HELPER_H_5BAA5D36_47EE_4D3E_BD24_E3BF13A30200
#include "sdbus/compile.h"
namespace sdbus{
	enum  kLogLevel{
		kLogLevelEmergency = 0,	// ����,�������ۺ�������������д��־�ĳ���
		kLogLevelCritical,		// ����,Ĭ�ϻᵼ��ӳ���ļ�����ˢ��
		kLogLevelError,			// ����,����ĳ����������
		kLogLevelAlert,			// ����,
		kLogLevelWarn,			// ����
		kLogLevelNotice,		// ֪ͨ
		kLogLevelInfo,			// ��Ϣ
		kLogLevelDebug,			// ����
		kLogLevelDebugview,		// ����,��������ȼ���־,���ܵȼ���������,ֻ��kBaseConfigLogToDebugView����
		kLogLevelMax
	};
	SDBUSAPI void raise_exception(const char *errmsg_);
	SDBUSAPI void write_log(bool raise, int level, char* file, char* func, int line, const char* format, ...);

	class SDBUSAPI thread_name_t{
		const char*		m_file;
		const char*		m_func;
		int				m_line;
		unsigned int	m_tid;
		double			m_time;
		char			m_name[128];
	public:
		thread_name_t(unsigned int tid, const char* file, const char* func, int line, const char* format, ...);
		~thread_name_t();
	};
	class SDBUSAPI code_metrics_t{
		double	tick_start_;
		char	message_[1024];
	public:
		code_metrics_t(char* file, char* func, int line, const char* format, ...);
		~code_metrics_t();
	};
}
#define sdbus_code_metric(fmt,...)	sdbus::code_metrics_t __localCodeMetrics(__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__);
#define sdbuslog(level,fmt,...)	sdbus::write_log(false,level,__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__);
#define sdbus_thread_name_ex(tid,fmt,...)  sdbus::thread_name_t _thread_name_(tid,__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__)
#define sdbus_thread_name(fmt,...)  sdbus::thread_name_t _thread_name_(-1,__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__)


#endif
