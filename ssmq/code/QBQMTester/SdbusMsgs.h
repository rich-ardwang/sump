#pragma once
#include "ISSMQObs.h"
#include "ssQBQMProto.h"
using namespace QB_QM_Com;

class CSdbusMsgs
{
public:
    //QM从QB查询资金权限 请求结构
    void BuildQM2QBComsRightQueryMsg(const QM2QBComsRightQuery &in_info, const SdbusMsgHeader &in_head, sdbus::Message &out_msg);
    void BuildQM2QBComsRightQueryBody(const QM2QBComsRightQuery &in_info, sdbus::Message::Map &out_map);
    void ParseQM2QBComsRightQueryMsg(const sdbus::Message &in_msg, SdbusMsgHeader &out_header, QM2QBComsRightQuery &out_info);
    void ParseQM2QBComsRightQueryBody(const sdbus::Message::Map &in_map, QM2QBComsRightQuery &out_info);

    //QM从QB查询资金权限 应答结构
    void BuildQM2QBComsRightQueryAckMsg(const QM2QBComsRightQueryAck &in_info, const SdbusMsgHeader &in_head, sdbus::Message &out_msg);
    void BuildQM2QBComsRightQueryAckBody(const QM2QBComsRightQueryAck &in_info, sdbus::Message::Map &out_map);
    void ParseQM2QBComsRightQueryAckMsg(const sdbus::Message &in_msg, SdbusMsgHeader &out_header, QM2QBComsRightQueryAck &out_info);
    void ParseQM2QBComsRightQueryAckBody(const sdbus::Message::Map &in_map, QM2QBComsRightQueryAck &out_info);


    //QM从QB客户端查询本人白名单 应答结构
    void BuildQM2QBComsWhiteListQueryAckMsg(const QM2QBComsWhiteListQueryAck &in_info, const SdbusMsgHeader &in_head, sdbus::Message &out_msg);
    void BuildQM2QBComsWhiteListQueryAckBody(const QM2QBComsWhiteListQueryAck &in_info, sdbus::Message::Map &out_map);
    void ParseQM2QBComsWhiteListQueryAckMsg(const sdbus::Message &in_msg, SdbusMsgHeader &out_header, QM2QBComsWhiteListQueryAck &out_info);
    void ParseQM2QBComsWhiteListQueryAckMsgBody(const sdbus::Message::Map &in_map, QM2QBComsWhiteListQueryAck &out_info);


    void BuildLstComsAdmissionUnit(const std::list<ComsAdmissionUnit> &in_lst, sdbus::Message::Map& out_map);
    void ParseLstComsAdmissionUnit(const sdbus::Message::Map &in_map, std::list<ComsAdmissionUnit> &out_info);

    void BuildMsgHead(int in_errCode, const std::string &in_errMsg, int in_totalNum, int in_index, sdbus::Message::Map &out_map);
    void BuildMsgHead(const SdbusMsgHeader &in_head, sdbus::Message::Map &out_map);
    void ParseMsgHead(const sdbus::Message::Map &in_map, SdbusMsgHeader &out_head);
};
