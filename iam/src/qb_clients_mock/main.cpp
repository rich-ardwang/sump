#include <iostream>
#include "getopt.h"
#include "user_args.h"
#include "tokeninfo.h"

void mock_qb_clients(user_args_type& user_args);

const struct option long_options[] = {
    { "iothreads", required_argument, NULL, 'i' },
    { "workthreads", required_argument, NULL, 'w' },
    { "ip", required_argument, NULL, 'a' },
    { "port", required_argument, NULL, 'p' },
    { "ucluster", required_argument, NULL, 'u' },
    { "cluster", optional_argument, NULL, 'l' },
    { "name", optional_argument, NULL, 'n' },
    { "type", optional_argument, NULL, 't' },
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
        "qb_clients_mock [option]   option as below: \n"
        "  -i|--iothreads           Specify io-threads count.\n"
        "  -w|--workthreads         Specify work-threads count.\n"
        "  -a|--ip                  Specify server address.\n"
        "  -p|--port                Specify server port number.\n"
        "  -u|--ucluster	        Specify if using cluster or not.\n"
        "  -l|--cluster             Specify cluster address.\n"
        "  -n|--name                Specify server name if using cluster.\n"
        "  -t|--type                Specify server type if using cluster.\n"
        "  -m|--mode                Test mode: 0:test service performance, 1:test data.\n"
        "  -s|--speed               Request speed control.\n"
        "  -h|--help                Display help.\n"
        "  for example:             \n"
        "      qb_clients_mock -m0 -s80000 -i2 -w1 -a127.0.0.1 -p10005 -u0 \n"
        "      qb_clients_mock -m0 -s80000 -i2 -w1 -a127.0.0.1 -p10005 -u1 -l192.168.19.1 2837 -n\"cdh\" -t1\n"
        );
}

bool parse_argument(int argc, char** argv, user_args_type& user_args) {
    if (argc < 7) {
        usage();
        return false;
    }

    //opt_index is index in long_options of current element been scanned.
    int opt = 0, opt_index = 0;
    while ((opt = getopt_long(argc, argv, "i:w:a:p:u:l:n:t:m:s:h", long_options, &opt_index)) != -1) {
        switch (opt)
        {
        case 'i':
            user_args.io_threads = atoi(optarg);
            break;

        case 'w':
            user_args.work_threads = atoi(optarg);
            break;

        case 'a':
            user_args.server_addr = optarg;
            break;

        case 'p':
            user_args.port = optarg;
            break;

        case 'u':
            user_args.use_cluster = atoi(optarg);
            break;

        case 'l':
            user_args.cluster_addr = optarg;
            break;

        case 'n':
            user_args.name = optarg;
            break;

        case 't':
            user_args.type = atoi(optarg);
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

//share memory for DDS server
HANDLE dds_mutex = nullptr;
HANDLE dds_event = nullptr;
HANDLE dds_shared_file = nullptr;
LPVOID dds_share_buf = nullptr;

//share memory for gateway server
HANDLE gtw_mutex = nullptr;
HANDLE gtw_event = nullptr;
HANDLE gtw_shared_file = nullptr;
LPVOID gtw_share_buf = nullptr;

//share memory for http server
HANDLE http_mutex = nullptr;
HANDLE http_event = nullptr;
HANDLE http_shared_file = nullptr;
LPVOID http_share_buf = nullptr;

void send_to_dds(const trans_token_info &tk_info) {
    WaitForSingleObject(dds_mutex, INFINITE); //acquire mutex lock
    memcpy(dds_share_buf, &tk_info, sizeof(trans_token_info));
    ReleaseMutex(dds_mutex);                  //unlock
    SetEvent(dds_event); //notify DDS server to receive token info
}

void send_to_gtw(const trans_token_info &tk_info) {
    WaitForSingleObject(gtw_mutex, INFINITE); //acquire mutex lock
    memcpy(gtw_share_buf, &tk_info, sizeof(trans_token_info));
    ReleaseMutex(gtw_mutex);                  //unlock
    SetEvent(gtw_event); //notify gateway server to receive token info
}

void send_to_http(const trans_token_info &tk_info) {
    WaitForSingleObject(http_mutex, INFINITE); //acquire mutex lock
    memcpy(http_share_buf, &tk_info, sizeof(trans_token_info));
    ReleaseMutex(http_mutex);                  //unlock
    SetEvent(http_event); //notify QB web to receive token info
}

int main(int argc, char* argv[]) {
    user_args_type user_args = {};
    if (!parse_argument(argc, argv, user_args))
        return 1;

    if (user_args.test_mode) {
        //create share file handle
        dds_shared_file = CreateFileMapping(
            INVALID_HANDLE_VALUE,   //physical file handle
            NULL,                   //default security level
            PAGE_READWRITE,         //PAGE_READWRITE:r/w，PAGE_READONLY:r，PAGE_WRITECOPY:w
            0,                      //file size high bit
            MAP_VIEW_BUF_SIZE,      //file size low bit
            L"dds_share_mem"        //share memory name
        );
        if (nullptr == dds_shared_file) {
            std::cout << "Could not create file mapping object for DDS server!" << std::endl;
            return 1;
        }

        gtw_shared_file = CreateFileMapping(
            INVALID_HANDLE_VALUE,   //physical file handle
            NULL,                   //default security level
            PAGE_READWRITE,         //PAGE_READWRITE:r/w，PAGE_READONLY:r，PAGE_WRITECOPY:w
            0,                      //file size high bit
            MAP_VIEW_BUF_SIZE,      //file size low bit
            L"gtw_share_mem"        //share memory name
        );
        if (nullptr == gtw_shared_file) {
            std::cout << "Could not create file mapping object for gateway server!" << std::endl;
            return 1;
        }

        http_shared_file = CreateFileMapping(
            INVALID_HANDLE_VALUE,   //physical file handle
            NULL,                   //default security level
            PAGE_READWRITE,         //PAGE_READWRITE:r/w，PAGE_READONLY:r，PAGE_WRITECOPY:w
            0,                      //file size high bit
            MAP_VIEW_BUF_SIZE,      //file size low bit
            L"http_share_mem"       //share memory name
        );
        if (nullptr == http_shared_file) {
            std::cout << "Could not create file mapping object for QB web!" << std::endl;
            return 1;
        }

        //map view of file and get the share memory pointer
        dds_share_buf = MapViewOfFile(
            dds_shared_file,        //file map handle
            FILE_MAP_ALL_ACCESS,    //access mode: r/w
            0,                      //file offset high 32 bit
            0,                      //file offset low 32 bit
            MAP_VIEW_BUF_SIZE       //map view size
        );
        if (nullptr == dds_share_buf) {
            std::cout << "Could not create map view object for DDS server!" << std::endl;
            CloseHandle(dds_shared_file);
            return 1;
        }

        gtw_share_buf = MapViewOfFile(
            gtw_shared_file,        //file map handle
            FILE_MAP_ALL_ACCESS,    //access mode: r/w
            0,                      //file offset high 32 bit
            0,                      //file offset low 32 bit
            MAP_VIEW_BUF_SIZE       //map view size
        );
        if (nullptr == gtw_share_buf) {
            std::cout << "Could not create map view object for gateway server!" << std::endl;
            CloseHandle(gtw_shared_file);
            return 1;
        }

        http_share_buf = MapViewOfFile(
            http_shared_file,       //file map handle
            FILE_MAP_ALL_ACCESS,    //access mode: r/w
            0,                      //file offset high 32 bit
            0,                      //file offset low 32 bit
            MAP_VIEW_BUF_SIZE       //map view size
        );
        if (nullptr == http_share_buf) {
            std::cout << "Could not create map view object for QB web!" << std::endl;
            CloseHandle(http_shared_file);
            return 1;
        }

        //create mutex and event
        dds_mutex = CreateMutex(NULL, FALSE, L"dds_mutex");
        dds_event = CreateEvent(NULL, //security control
            //auto setting or manual setting，TRUE:manual，FALSE:auto.
            //if auto setting， it will call ResetEvent() after calling WaitForSingleObject()
            FALSE,
            FALSE,       //event ini status，TRUE:triggered
            L"dds_event" //event name. if no event name setting NULL
        );
        gtw_mutex = CreateMutex(NULL, FALSE, L"gtw_mutex");
        gtw_event = CreateEvent(NULL, //security control
            //auto setting or manual setting，TRUE:manual，FALSE:auto.
            //if auto setting， it will call ResetEvent() after calling WaitForSingleObject()
            FALSE,
            FALSE,       //event ini status，TRUE:triggered
            L"gtw_event" //event name. if no event name setting NULL
        );
        http_mutex = CreateMutex(NULL, FALSE, L"http_mutex");
        http_event = CreateEvent(NULL, //security control
            //auto setting or manual setting，TRUE:manual，FALSE:auto.
            //if auto setting， it will call ResetEvent() after calling WaitForSingleObject()
            FALSE,
            FALSE,       //event ini status，TRUE:triggered
            L"http_event" //event name. if no event name setting NULL
       );
    }

    //start to the test module
    mock_qb_clients(user_args);

    if (user_args.test_mode) {
        //close mutex and event handle
        CloseHandle(dds_mutex);
        CloseHandle(dds_event);
        CloseHandle(gtw_mutex);
        CloseHandle(gtw_event);
        CloseHandle(http_mutex);
        CloseHandle(http_event);

        //release map view and close handle
        UnmapViewOfFile(dds_share_buf);
        UnmapViewOfFile(gtw_share_buf);
        UnmapViewOfFile(http_share_buf);
        CloseHandle(dds_shared_file);
        CloseHandle(gtw_shared_file);
        CloseHandle(http_shared_file);
    }

    std::cout << "\nPress enter key to exit." << std::endl;
    std::cin.get();
    return 0;
}