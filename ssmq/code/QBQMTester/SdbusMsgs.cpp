#include "stdafx.h"
#include "SdbusMsgs.h"
#include <string>]
#include "ISSMQMng.h"
using namespace std;

void CSdbusMsgs::BuildMsgHead( int in_errCode, const std::string &in_errMsg, int in_totalNum, int in_index, sdbus::Message::Map &out_map )
{
    out_map.SetInt32(1, in_errCode);
    out_map.SetString(2, in_errMsg);
    out_map.SetInt32(3, in_totalNum);
    out_map.SetInt32(4, in_index);
}

void CSdbusMsgs::BuildMsgHead( const SdbusMsgHeader &in_head, sdbus::Message::Map &out_map )
{
    out_map.SetInt32(1, in_head.errorCode);
    out_map.SetString(2, in_head.errorMsg);
    out_map.SetInt32(3, in_head.totalNum);
    out_map.SetInt32(4, in_head.index);
}

void CSdbusMsgs::ParseMsgHead( const sdbus::Message::Map &in_map, SdbusMsgHeader &out_head )
{
    in_map.GetInt32(1, out_head.errorCode);
    in_map.GetString(2, out_head.errorMsg);
    in_map.GetInt32(3, out_head.totalNum);
    in_map.GetInt32(4, out_head.index);
}

void CSdbusMsgs::BuildQM2QBComsRightQueryMsg( const QM2QBComsRightQuery &in_info, const SdbusMsgHeader &in_head, sdbus::Message &out_msg )
{
    out_msg.SetType(SSCMD_ComsRightQuery);//业务类型

    //包头
    sdbus::Message::Map msgHead, msgBody;    
    BuildMsgHead(in_head, msgHead);
    out_msg.SetMap(1, msgHead);
    //包体
    BuildQM2QBComsRightQueryBody(in_info, msgBody);
    out_msg.SetMap(2, msgBody);
}

void CSdbusMsgs::BuildQM2QBComsRightQueryBody( const QM2QBComsRightQuery &in_info, sdbus::Message::Map &out_map )
{
    out_map.SetString(1, in_info.sUserID);
}

void CSdbusMsgs::ParseQM2QBComsRightQueryMsg( const sdbus::Message &in_msg, SdbusMsgHeader &out_header, QM2QBComsRightQuery &out_info )
{
    sdbus::Message::Map msgHead, msgBody;

    //包头解析
    in_msg.GetMap(1, msgHead);
    ParseMsgHead(msgHead, out_header);
    //包体解析
    in_msg.GetMap(2, msgBody);
    ParseQM2QBComsRightQueryBody(msgBody, out_info);
}

void CSdbusMsgs::ParseQM2QBComsRightQueryBody( const sdbus::Message::Map &in_map, QM2QBComsRightQuery &out_info )
{
    in_map.GetString(1, out_info.sUserID);
}

void CSdbusMsgs::BuildQM2QBComsRightQueryAckMsg( const QM2QBComsRightQueryAck &in_info, const SdbusMsgHeader &in_head, sdbus::Message &out_msg )
{
    out_msg.SetType(SSCMD_ComsRightQuery);//业务类型

    //包头
    sdbus::Message::Map msgHead, msgBody;    
    BuildMsgHead(in_head, msgHead);
    out_msg.SetMap(1, msgHead);
    //包体
    BuildQM2QBComsRightQueryAckBody(in_info, msgBody);
    out_msg.SetMap(2, msgBody);
}

void CSdbusMsgs::BuildQM2QBComsRightQueryAckBody( const QM2QBComsRightQueryAck &in_info, sdbus::Message::Map &out_map )
{
    out_map.SetString(1, in_info.sUserID);
    out_map.SetString(2, in_info.sValue);
}

void CSdbusMsgs::ParseQM2QBComsRightQueryAckMsg( const sdbus::Message &in_msg, SdbusMsgHeader &out_header, QM2QBComsRightQueryAck &out_info )
{
    sdbus::Message::Map msgHead, msgBody;

    //包头解析
    in_msg.GetMap(1, msgHead);
    ParseMsgHead(msgHead, out_header);
    //包体解析
    in_msg.GetMap(2, msgBody);
    ParseQM2QBComsRightQueryAckBody(msgBody, out_info);
}

void CSdbusMsgs::ParseQM2QBComsRightQueryAckBody( const sdbus::Message::Map &in_map, QM2QBComsRightQueryAck &out_info )
{
    in_map.GetString(1, out_info.sUserID);
    in_map.GetString(2, out_info.sValue);
}

void CSdbusMsgs::BuildQM2QBComsWhiteListQueryAckMsg( const QM2QBComsWhiteListQueryAck &in_info, const SdbusMsgHeader &in_head, sdbus::Message &out_msg )
{
    out_msg.SetType(SSCMD_ComsWhiteListQuery);//业务类型

    //包头
    sdbus::Message::Map msgHead, msgBody;    
    BuildMsgHead(in_head, msgHead);
    out_msg.SetMap(1, msgHead);
    //包体
    BuildQM2QBComsWhiteListQueryAckBody(in_info, msgBody);
    out_msg.SetMap(2, msgBody);
}

void CSdbusMsgs::BuildQM2QBComsWhiteListQueryAckBody( const QM2QBComsWhiteListQueryAck &in_info, sdbus::Message::Map &out_map )
{
    out_map.SetString(1, in_info.sUserID);

    sdbus::Message::Map map;
    BuildLstComsAdmissionUnit(in_info.lstWhite, map);
    out_map.SetMap(2, map);
}

void CSdbusMsgs::BuildLstComsAdmissionUnit( const std::list<ComsAdmissionUnit> &in_lst, sdbus::Message::Map& out_map )
{
    int pos(1);
    for (std::list<ComsAdmissionUnit>::const_iterator iter = in_lst.begin(); iter != in_lst.end(); ++iter)
    {
        sdbus::Message::Map Item;
        Item.SetString(1, iter->sCompanyID);
        Item.SetString(2, iter->sValue);

        out_map.SetMap(pos, Item);
        ++pos;
    }
}


void CSdbusMsgs::ParseQM2QBComsWhiteListQueryAckMsg( const sdbus::Message &in_msg, SdbusMsgHeader &out_header, QM2QBComsWhiteListQueryAck &out_info )
{
    sdbus::Message::Map msgHead, msgBody;

    //包头解析
    in_msg.GetMap(1, msgHead);
    ParseMsgHead(msgHead, out_header);
    //包体解析
    in_msg.GetMap(2, msgBody);
    ParseQM2QBComsWhiteListQueryAckMsgBody(msgBody, out_info);
}

void CSdbusMsgs::ParseQM2QBComsWhiteListQueryAckMsgBody( const sdbus::Message::Map &in_map, QM2QBComsWhiteListQueryAck &out_info )
{
    in_map.GetString(1, out_info.sUserID);

    sdbus::Message::Map map;
    in_map.GetMap(2, map);
    ParseLstComsAdmissionUnit(map, out_info.lstWhite);
}

void CSdbusMsgs::ParseLstComsAdmissionUnit( const sdbus::Message::Map &in_map, std::list<ComsAdmissionUnit> &out_info )
{
    for (size_t i = 0; i < in_map.Size(); ++i)
    {
        sdbus::Message::Map mpItem;
        in_map.GetMap(i+1, mpItem);

        bool bSuccess = true;
        ComsAdmissionUnit ParseOut;
        bSuccess &= mpItem.GetString(1, ParseOut.sCompanyID);
        bSuccess &= mpItem.GetString(2, ParseOut.sValue);

        out_info.push_back(ParseOut);
    }
}
