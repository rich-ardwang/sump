/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : main.cpp
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-17
*Desc        : the entrance of login server.
*History     : Add by Lei.Wang 2019-09-17
*            : Mod by Lei.Wang 2019-10-23
***************************************************************************************************/
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "lava_utils_api.h"
#include "msgbus_server.h"
#include "lava_server.h"
#include "http_server.h"

#ifndef LAVA_WIN
#include <signal.h>
#endif

#define main_moduler        "iam.login_server.main"

extern lava_utils_api void release_lava_config();
extern iam::login_server::i_msgbus_server *get_msgbus_server();
extern void release_msgbus_server();
extern iam::login_server::i_lava_server *get_lava_server();
extern void release_lava_server();
extern iam::login_server::i_http_server *get_http_server();
extern void release_http_server();
extern void release_server_cache();
extern void release_lava_process();
extern void release_msgbus_process();

#ifndef LAVA_WIN
bool g_srv_exit_ = false;
//handle function for linux system signal
void sys_sig_action(int nSig) {
    switch (nSig) {
    case SIGINT:    //Ctrl+C
    case SIGTERM:   //kill pid (default kill signal)
        g_srv_exit_ = true;
        log_info(main_moduler, "received signal to exit server, sigid:%d.", nSig);
        break;

    default:
        log_info(main_moduler, "received signal which no need handle, sigid:%d.", nSig);
    }
}
#endif

int main() {
    /* prepare to start login server */
    log_info(main_moduler, "login_server start to boot.");

    /* config init */
    get_lava_config()->init();
    log_info(main_moduler, "initial config complete.");

    /* wait until loading config complete */
    while (get_lava_config()->fail()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    log_info(main_moduler, "loading config complete.");

#ifndef LAVA_WIN
    /* register signal to handle callback */
    signal(SIGINT, sys_sig_action);
    signal(SIGTERM, sys_sig_action);
    log_info(main_moduler, "register signal complete.");
#endif

    /* msgbus server module init */
    get_msgbus_server()->start();

    /* lava server module init */
    get_lava_server()->start();

    /* http server module init */
    get_http_server()->start();

    /* main thread sleep */
    log_info(main_moduler, "login server main thread start to sleeping loop.");
#ifdef LAVA_WIN
    int i = 0;
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        i++;
        if (100000 == i) {
            //stop server
            get_lava_server()->stop();
            get_http_server()->stop();
            break;
        }
    }
#else
    while (true) {
        //handle server exit
        if (g_srv_exit_) {
            //stop server
            get_lava_server()->stop();
            get_http_server()->stop();
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
#endif

    /* release modules */
    release_lava_config();
    release_msgbus_server();
    release_lava_server();
    release_http_server();
    release_lava_process();
    release_msgbus_process();
    release_server_cache();

#ifdef LAVA_WIN
    std::cout << "Press enter key to exit." << std::endl;
    std::cin.get();
#endif
    return 0;
}

