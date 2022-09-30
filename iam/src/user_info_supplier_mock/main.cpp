// user_info_supplier_mock.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "lava_utils_api.h"
#include "msgbus_thread_factory.h"
#include "session.h"
#include "getopt.h"
#include "user_args.h"

#define user_info_supplier_mock         "iam.test.user_info_supplier.main"

const struct option long_options[] = {
    { "server_addr", required_argument, NULL, 'a' },
    { "crt_data_cnt", required_argument, NULL, 'c' },
    { 0, 0, 0, 0 }
};

void usage() {
    //
    // Don't use TAB since it is different width on different platform.
    // Only use space to align.
    //
    fprintf(stderr,
        "user_info_supplier_mock [option]   option as below: \n"
        "  -a|--ip&port             Specify server address.\n"
        "  -c|--data count          Create user info count.\n"
        "  -h|--help                Display help.\n"
        "  for example:             \n"
        "      dds_server_mock -a172.16.8.85 -c100 \n"
        );
}

bool parse_argument(int argc, char** argv, user_args_type& user_args) {
    if (argc < 3) {
        usage();
        return false;
    }

    //opt_index is index in long_options of current element been scanned.
    int opt = 0, opt_index = 0;
    while ((opt = getopt_long(argc, argv, "a:c:h", long_options, &opt_index)) != -1) {
        switch (opt)
        {
        case 'a':
            user_args.server_addr = optarg;
            break;

        case 'c':
            user_args.create_user_info_cnt = atoi(optarg);
            break;

        case 'h':
            usage();
            return false;

        default:
            usage();
            return false;
        }
    }

    return true;
}

void create_and_package_data(__lv_in FanoutUserInfoPtr uinfo_ptr, 
    __lv_out std::shared_ptr<msgbus::Message> msg) {
    //check input
    if ((nullptr == uinfo_ptr) || uinfo_ptr->empty() || (nullptr == msg))
        return;

    //set header: type and login server id
    msg->setType(DB_SRV_USER_INFO_UPD_FANOUT);

    //set body
    msgbus::VariantMap *bodyMap = msg->AddMap("xUserInfoUpdFanout");
    if (bodyMap) {
        msgbus::VariantList *lst = bodyMap->AddList("List");
        if (lst) {
            for (auto &it : *uinfo_ptr) {
                msgbus::VariantMap *map = lst->AddMap();
                if (map) {
                    map->setInt16("optFlg", it.opt_flag_);
                    map->setString("userName", it.user_name_);
                    map->setString("userPwd", it.user_pwd_);
                    map->setString("userId", it.user_id_);
                    map->setString("cliMacAddr", it.cli_mac_addr_);
                    map->setString("cliVersion", it.cli_version_);
                }
            }
        }
    }
}

#define user_info_data_cnt                  100

int main(int argc, char* argv[]) {
    user_args_type user_args = {};
    if (!parse_argument(argc, argv, user_args))
        return 1;

    if (user_args.create_user_info_cnt < 100)
        user_args.create_user_info_cnt = user_info_data_cnt;

    std::string drt_ip(user_args.server_addr);
    if (drt_ip.empty())
        drt_ip = "172.16.8.85";
    std::string drt_lsn_q("");
    boost::shared_ptr<UserinfoSession> bus_ss(boost::make_shared<UserinfoSession>());
    boost::shared_ptr<MsgbusThread> fanout_worker_ = MsgbusThreadFactory::concrete(kFanoutMsgbus, drt_ip, drt_lsn_q, bus_ss);
    if (nullptr == fanout_worker_) {
        log_error(user_info_supplier_mock, "message bus fanout worker start failed!");
        return false;
    }
    log_info(user_info_supplier_mock, "connect to msgbus complete.");

    FanoutUserInfoPtr fout_ptr(std::make_shared<std::vector<user_info> >());
    for (int i = 0; i < user_args.create_user_info_cnt; i++) {
        user_info tmp;
        tmp.opt_flag_ = 1;
        tmp.user_name_ = "id000" + std::to_string(i);
        tmp.user_pwd_ = std::to_string(i) + ".com";
        tmp.user_id_ = "user_id_" + std::to_string(i);
        tmp.cli_mac_addr_ = "cli_mac_addr_" + std::to_string(i);
        tmp.cli_version_ = "cli_version_" + std::to_string(i);
        fout_ptr->push_back(tmp);
    }
    log_info(user_info_supplier_mock, "create userinfo count:%d.", user_args.create_user_info_cnt);

    std::shared_ptr<msgbus::Message> msgbus_msg_ptr(std::make_shared<msgbus::Message>());
    create_and_package_data(fout_ptr, msgbus_msg_ptr);
    if (nullptr == msgbus_msg_ptr) {
        log_error(user_info_supplier_mock, "no message!");
        return -1;
    }
    log_info(user_info_supplier_mock, "message package complete.");

    std::string queue_name = "login_srv_fanout_listen.queue";
    fanout_worker_->fanoutMessage(queue_name, *msgbus_msg_ptr);
    log_info(user_info_supplier_mock, "message send complete.");

    std::cout << "enter to quit." << std::endl;
    std::cin.get();
    return 0;
}
