/*
 *	dds_def.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds API common define
*/
#pragma once
#include <sfa/sfa_macro.h>
#include <sfa/msg/msg.h>
#include <sfa/msg/msg_codec.h>
#include <sfa/msg/value_field.h>
#include <sfa/msg/value_array.h>
#include <sfa/msg/value_array_row.h>
#include <sfa/msg/value_ascii.h>
#include <sfa/msg/value_byte.h>
#include <sfa/msg/value_date.h>
#include <sfa/msg/value_decimal.h>
#include <sfa/msg/value_integer.h>
#include <sfa/msg/value_kv_array.h>
#include <sfa/msg/value_kv_array_row.h>
#include <sfa/msg/value_time.h>
#include <sfa/msg/value_unicode.h>
#include <sfa/msg/value_unsigned_integer.h>
#include <sfa_utils/sfa_base.h>

namespace sfa {
namespace dds {

//�������������
enum class ProxyType : uint8_t {
	proxy_none = 0,		//��ʹ�ô���
	proxy_socks4 = 1,	//socket4����
	proxy_socks5 = 2,	//socket5����
	proxy_http = 3		//http����
};

//�����������Ϣ
typedef struct {
	ProxyType type;			//����
	const char* host;		//������ַ
	const char* port;		//�˿�
	const char* username;	//�û���
	const char* password;	//����
}ClientProxyInfo;

//�ͻ��˵�¼��Ϣ
typedef struct {
	bool use_proxy;				//�Ƿ�ʹ�ô���
	ClientProxyInfo* proxy_info;//������Ϣ
	const char* host;			//������ַ
	const char* port;			//�˿�
	const char* username;		//�û���
	const char* password;		//����
}ClientLoginInfo;

//��ʷ��������
enum class Period {
	one_minute = 1,		//1����
	five_minutes = 2,	//5����
	thirty_minutes = 3,	//30����
	one_hour = 4,		//1Сʱ
	day = 5,			//��
	week = 6,			//��
	month = 7,			//��
	quarter = 8,		//��
	year = 9			//��
};

}
}