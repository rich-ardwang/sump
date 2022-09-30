#include <atomic>
#include "session.h"
#include "lava_utils_api.h"
#include "user_args.h"

typedef msgbus::VariantList sslist;
typedef msgbus::VariantMap ssmap;

extern std::atomic<uint32_t>       g_replys;
extern user_args_type              user_args;

void GtwServerSession::onMessage(const Message &message) {
    if (!user_args.test_mode)
        ++g_replys;
    else {
        msgbus::Message::Map ackBodyMap;
        message.GetMap("AckMsgBody", ackBodyMap);
        if (ackBodyMap.empty()) {
            log_warn(session_moduler, "ack body map parse failed!");
            return;
        }

        msgbus::Message::List lstAck;
        ackBodyMap.getList("List", lstAck);
        if (lstAck.empty()) {
            log_warn(session_moduler, "ack body list list parse failed!");
            return;
        }

        //parse ack msg content map
        const ssmap* pMap = nullptr;
        msgbus::Message::List::const_iterator itLst = lstAck.begin();
        for (; itLst != lstAck.end(); itLst++) {
            if (!itLst->RefMap(pMap) || nullptr == pMap)
                continue;
            //get data
            short retCode = 0;
            std::string retMsg;
            std::string u_name;
            std::string u_id;
            std::string token;
            std::string srv_time;
            pMap->getInt16("verifyRet", retCode);
            pMap->getString("verifyMsg", retMsg);
            pMap->getString("userName", u_name);
            pMap->getString("userId", u_id);
            pMap->getString("token", token);
            pMap->getString("serverTime", srv_time);

            log_debug(session_moduler, "retCode:%d,retMsg:%s,userName:%s,userId:%s,token:%s,serverTime:%s.", 
                retCode, retMsg.c_str(), u_name.c_str(), u_id.c_str(), token.c_str(), srv_time.c_str());
        }
    }
}
