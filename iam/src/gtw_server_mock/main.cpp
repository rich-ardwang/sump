#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "lava_utils_api.h"
#include "msgbus_thread_factory.h"
#include "session.h"
#include "tokeninfo.h"
#include "timer.h"
#include "getopt.h"
#include "user_args.h"

#define gtw_server_mock         "iam.test.gtw_server_mock.main"
#define default_send_queue      "gateway_srv_token_verify.req.queue"
#define default_listen_queue    "gateway_srv_token_verify.req.queue.rspa"
#define default_req_limit       20000

const struct option long_options[] = {
    { "ip", required_argument, NULL, 'a' },
    { "send queue", required_argument, NULL, 'd' },
    { "listen queue", required_argument, NULL, 'l' },
    { "cluster", optional_argument, NULL, 'l' },
    { "mode", optional_argument, NULL, 'm' },
    { "speed", optional_argument, NULL, 's' },
    { "help", no_argument, NULL, 'h' },
    { 0, 0, 0, 0 }
};

//
// Print usage guide for user
//
void usage() {
    //
    // Don't use TAB since it is different width on different platform.
    // Only use space to align.
    //
    fprintf(stderr,
        "gtw_server_mock [option]   option as below: \n"
        "  -a|--ip&port             Specify server address.\n"
        "  -d|--send queue          Send queue name.\n"
        "  -l|--listen queue        Listen queue name.\n"
        "  -m|--mode                Test mode: 0:test service performance, 1:test data.\n"
        "  -s|--speed               Request speed control.\n"
        "  -h|--help                Display help.\n"
        "  for example:             \n"
        "      gtw_server_mock -a172.16.8.85 -dgateway_srv_token_verify.req.queue \
                                   -lgateway_srv_token_verify.req.queue.rspa -m0 -s2000 \n"
                                   );
}

bool parse_argument(int argc, char** argv, user_args_type& user_args) {
    if (argc < 6) {
        usage();
        return false;
    }

    //opt_index is index in long_options of current element been scanned.
    int opt = 0, opt_index = 0;
    while ((opt = getopt_long(argc, argv, "a:d:l:m:s:h", long_options, &opt_index)) != -1) {
        switch (opt)
        {
        case 'a':
            user_args.server_addr = optarg;
            break;

        case 'd':
            user_args.send_queue = optarg;
            break;

        case 'l':
            user_args.listen_queue = optarg;
            break;

        case 'm':
            user_args.test_mode = atoi(optarg);
            break;

        case 's':
            user_args.req_speed = atoi(optarg);
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

//share memory for gateway server
HANDLE gtw_mutex = nullptr;
HANDLE gtw_event = nullptr;
HANDLE gtw_shared_file = nullptr;
LPVOID gtw_share_buf = nullptr;

void parse_token_info(trans_token_info &tk_info) {
    WaitForSingleObject(gtw_event, INFINITE); //wait gateway server read signal
    WaitForSingleObject(gtw_mutex, INFINITE); //acquire mutex lock
    memset(&tk_info, 0, sizeof(trans_token_info));
    memcpy(&tk_info, gtw_share_buf, sizeof(trans_token_info));
    ReleaseMutex(gtw_mutex);                  //unlock
}

void package_data(__lv_in VerifyTokenReqPtr req_ptr, 
    __lv_out std::shared_ptr<msgbus::Message> msg, std::string &ltn_q) {
    //check input
    if ((nullptr == req_ptr) || req_ptr->vec_srv_auth_req_.empty() || (nullptr == msg))
        return;

    //set header: type and replyto
    msg->setType(GTW_SRV_VERIFY_TOKEN_REQ);
    msg->setReplyTo(ltn_q);

    //set body
    msgbus::VariantMap *bodyMap = msg->AddMap("xGtwTokenVerifyReq");
    if (bodyMap) {
        msgbus::VariantList *lst = bodyMap->AddList("List");
        if (lst) {
            for (auto &it : req_ptr->vec_srv_auth_req_) {
                msgbus::VariantMap *map = lst->AddMap();
                if (map) {
                    map->setString("userName", it.user_name_);
                    map->setString("token", it.token_);
                }
            }
        }
    }
}

lava::utils::timer          g_timer;
uint32_t                    g_interval = 2;
std::atomic<uint32_t>       g_replys = 0;
std::atomic<uint32_t>       g_requests = 0;
user_args_type              user_args = {};

void timer_request_cb() {
    uint32_t replys = g_replys;
    uint32_t requests = g_requests;
    g_replys = 0;
    g_requests = 0;
    log_info(gtw_server_mock, "replys average = %d; requests average = %d.", replys / g_interval, requests / g_interval);
}

int main(int argc, char* argv[]) {
    if (!parse_argument(argc, argv, user_args))
        return 1;

    if (user_args.test_mode) {
        //open share file handle
        gtw_shared_file = OpenFileMapping(
            SECTION_MAP_READ,           //access mode: read
            FALSE,
            L"gtw_share_mem"            //share memory name
        );
        if (nullptr == gtw_shared_file) {
            std::cout << "Could not open file mapping object for gateway server!" << std::endl;
            return 1;
        }

        //map view of file and get the share memory pointer
        gtw_share_buf = MapViewOfFile(
            gtw_shared_file,        //file map handle
            FILE_MAP_READ,          //access mode: read
            0,                      //file offset high 32 bit
            0,                      //file offset low 32 bit
            //map view size, 0:from offset to the end of file map.
            //because process don't know the size£¬ set 0
            0
        );
        if (nullptr == gtw_share_buf) {
            std::cout << "Could not open file mapping object!" << std::endl;
            CloseHandle(gtw_shared_file);
            return 1;
        }

        //open the created mutex
        gtw_mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"gtw_mutex");
        if (nullptr == gtw_mutex) {
            std::cout << "open gtw mutex failed!" << std::endl;
            return 1;
        }

        //open the created event handle
        gtw_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"gtw_event");
        if (nullptr == gtw_event) {
            std::cout << "open gtw event failed!" << std::endl;
            return 1;
        }
    }

    //connect to msgbus
    std::string drt_ip(user_args.server_addr);
    if (drt_ip.empty())
        drt_ip = "172.16.8.85";
    std::string lsn_queue(user_args.listen_queue);
    if (lsn_queue.empty())
        lsn_queue = default_listen_queue;
    std::string send_queue(user_args.send_queue);
    if (send_queue.empty())
        send_queue = default_send_queue;
    boost::shared_ptr<GtwServerSession> bus_ss(boost::make_shared<GtwServerSession>());
    boost::shared_ptr<MsgbusThread> dt_worker_ = MsgbusThreadFactory::concrete(kDirectMsgbus, drt_ip, lsn_queue, bus_ss);
    if (nullptr == dt_worker_) {
        log_error(gtw_server_mock, "message bus direct worker start failed!");
        return false;
    }
    log_info(gtw_server_mock, "connect to msgbus complete.");

    if (!user_args.test_mode)
        //start timer
        g_timer.start(g_interval * 1000, std::bind(&timer_request_cb));

    //create and send token verify request message
    if (0 == user_args.req_speed)
        user_args.req_speed = default_req_limit;
    while (true) {
        if (g_requests > user_args.req_speed)
            continue;

        VerifyTokenReqPtr vfy_req_ptr(std::make_shared<verify_token_req>());
        server_auth_req auth_req;
        if (user_args.test_mode) {
            trans_token_info tmp_tkinfo;
            parse_token_info(tmp_tkinfo);
            auth_req.user_name_ = tmp_tkinfo.u_name;
            auth_req.token_ = tmp_tkinfo.token;
        }
        else {
            auth_req.user_name_ = "id0001";
            auth_req.token_ = "336df8681794f308f244da4532ad07efe73ee39f3f4fd5c42177288914da70e0";
        }
        vfy_req_ptr->vec_srv_auth_req_.push_back(auth_req);
        std::shared_ptr<msgbus::Message> msgbus_msg_ptr(std::make_shared<msgbus::Message>());
        package_data(vfy_req_ptr, msgbus_msg_ptr, lsn_queue);
        if (nullptr == msgbus_msg_ptr) {
            log_warn(gtw_server_mock, "no message!");
            continue;
        }
        dt_worker_->sendMessage(send_queue, *msgbus_msg_ptr);
        ++g_requests;

        if (user_args.test_mode)
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (user_args.test_mode) {
        //close mutex and event handle
        CloseHandle(gtw_mutex);
        CloseHandle(gtw_event);

        //release map view and close handle
        UnmapViewOfFile(gtw_share_buf);
        CloseHandle(gtw_shared_file);
    }
    else
        //close timer
        g_timer.expire();

    std::cout << "enter to quit." << std::endl;
    std::cin.get();
    return 0;
}
