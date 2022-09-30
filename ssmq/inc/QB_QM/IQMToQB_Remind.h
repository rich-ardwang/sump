#pragma once

#include "QB_QMDefine.h"

using std::string;
//QM������ص������ӿ�

namespace QB_QM
{
    //�ص��ӿ�,��QMʵ��
    class IQMToQBCallback_Remind
    {
    protected:
        IQMToQBCallback_Remind(){}

    public:
        virtual ~IQMToQBCallback_Remind(){}

        //��ѯ��ǰ���õ�QM����
        virtual void QueryQMRemindConfig(const string& sMsgID) = 0;
        //����QM�ؼ�������
        virtual void AddQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item) = 0;
        //�޸�QM�ؼ�������
        virtual void ModifyQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item) = 0;
        //ɾ��QM�ؼ�������
        virtual void DelQMRemindKeyItem(const string& sMsgID, const SetString& setID) = 0;
        //����QM��������
        virtual void AddQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item) = 0;
        //�޸�QM��������
        virtual void ModifyQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item) = 0;
        //ɾ��QM��������
        virtual void DelQMRemindFriendItem(const string& sMsgID, const SetString& setID) = 0;
        //����QM���ѷ�ʽ
        virtual void SetQMRemindType(const string& sMsgID, unsigned int nRemindType) = 0;
    };

    class IQMToQB_Remind
    {
    protected:
        IQMToQB_Remind(){}

    public:
        virtual ~IQMToQB_Remind(){}

        //ע��Bond��ػص��ӿ�
        virtual void RegCallback(IQMToQBCallback_Remind* pCallback) = 0;
        //��ѯ��ǰ���õ�QM���ѷ���
        virtual void QueryQMRemindConfigRet(const string& sMsgID, const QMRemindConfig& QMRCfg) = 0;
        //����QM�ؼ������ѷ���
        virtual void AddQMRemindKeyItemRet(const string& sMsgID, const string& ItemID) = 0;
        //�޸�QM�ؼ������ѷ���
        virtual void ModifyQMRemindKeyItemRet(const string& sMsgID, bool bSuccess) = 0;
        //ɾ��QM�ؼ������ѷ���
        virtual void DelQMRemindKeyItemRet(const string& sMsgID, const SetString& setSuccessID) = 0;
        //����QM�������ѷ���
        virtual void AddQMRemindFriendItemRet(const string& sMsgID, const string& ItemID) = 0;
        //�޸�QM�������ѷ���
        virtual void ModifyQMRemindFriendItemRet(const string& sMsgID, bool bSuccess) = 0;
        //ɾ��QM�������ѷ���
        virtual void DelQMRemindFriendItemRet(const string& sMsgID, const SetString& setSuccessID) = 0;
        //����QM���ѷ�ʽ����
        virtual void SetQMRemindTypeRet(const string& sMsgID, bool bSuccess) = 0;
    };
}


