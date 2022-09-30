/*
 *	dds_api.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds API define
*/
#pragma once
#include <sfa/dds/dds_def.h>
#include <sfa/dds/dds_proxy.h>

namespace sfa {
namespace dds {

/*
* ��  �ܣ���ʼ��dds APIģ��
* ��  ����
*	config_file �����ļ�·��
* ����ֵ��true�ɹ� falseʧ��
*/
SFA_API bool Init(_sfa_in_ const char* config_file);

/*
* ��  �ܣ������Զ���������������
* ��  ����
*	is_auto �Ƿ��Զ�����
*	try_times ���Դ���
* ����ֵ����
*/
SFA_API void SetAutoReconnect(_sfa_in_ bool is_auto, _sfa_in_ int try_times);

/*
* ��  �ܣ���¼dds server
* ��  ����
*	client_login_info �ͻ�����Ϣ
*	listener �����¼�����
* ����ֵ����
*/
SFA_API void Login(_sfa_in_ const ClientLoginInfo* login_info, _sfa_in_ const IEventListener *listener);

/*
* ��  �ܣ���ȡddsҵ��ӿ�
* ��  ������
* ����ֵ��ddsҵ��ӿ�ָ��
*/
SFA_API IDdsProxy* CreateProxy(void);

/*
* ��  �ܣ��ͷ�ddsҵ��ӿ�ָ��
* ��  ����
*	proxy CreateProxy�������ص�ָ���ַ
* ����ֵ����
*/
SFA_API void ReleaseProxy(_sfa_inout_ IDdsProxy** proxy);

/*
* ��  �ܣ��ǳ�dds server
* ��  ������
* ����ֵ����
*/
SFA_API void Logout(void);

/*
* ��  �ܣ��ͷ�dds APIģ��
* ��  ������
* ����ֵ����
*/
SFA_API void Uninit(void);

}
}
