#include <iostream>
#include "getopt.h"
#include "user_args.h"
#include "tokeninfo.h"

void mock_dds_server(user_args_type& user_args);

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
        "dds_server_mock [option]   option as below: \n"
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
        "      dds_server_mock -m0 -s80000 -i2 -w1 -a127.0.0.1 -p10005 -u0 \n"
        "      dds_server_mock -m0 -s80000 -i2 -w1 -a127.0.0.1 -p10005 -u1 -l192.168.19.1 2837 -n\"cdh\" -t1\n"
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

void parse_token_info(trans_token_info &tk_info) {
    WaitForSingleObject(dds_event, INFINITE); //wait DDS server read signal
    WaitForSingleObject(dds_mutex, INFINITE); //acquire mutex lock
    memset(&tk_info, 0, sizeof(trans_token_info));
    memcpy(&tk_info, dds_share_buf, sizeof(trans_token_info));
    ReleaseMutex(dds_mutex);                  //unlock
}

int main(int argc, char* argv[]) {
    user_args_type user_args = {};
    if (!parse_argument(argc, argv, user_args))
        return 1;

    if (user_args.test_mode) {
        //open share file handle
        dds_shared_file = OpenFileMapping(
            SECTION_MAP_READ,           //access mode: read
            FALSE,
            L"dds_share_mem"            //share memory name
        );
        if (nullptr == dds_shared_file) {
            std::cout << "Could not open file mapping object for DDS server!" << std::endl;
            return 1;
        }

        //map view of file and get the share memory pointer
        dds_share_buf = MapViewOfFile(
            dds_shared_file,        //file map handle
            FILE_MAP_READ,          //access mode: read
            0,                      //file offset high 32 bit
            0,                      //file offset low 32 bit
            //map view size, 0:from offset to the end of file map.
            //because process don't know the size， set 0
            0
        );
        if (nullptr == dds_share_buf) {
            std::cout << "Could not open file mapping object!" << std::endl;
            CloseHandle(dds_shared_file);
            return 1;
        }

        //open the created mutex
        dds_mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"dds_mutex");
        if (nullptr == dds_mutex) {
            std::cout << "open dds mutex failed!" << std::endl;
            return 1;
        }

        //open the created event handle
        dds_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"dds_event");
        if (nullptr == dds_event) {
            std::cout << "open dds event failed!" << std::endl;
            return 1;
        }
    }

    //start to the test module
    mock_dds_server(user_args);

    if (user_args.test_mode) {
        //close mutex and event handle
        CloseHandle(dds_mutex);
        CloseHandle(dds_event);

        //release map view and close handle
        UnmapViewOfFile(dds_share_buf);
        CloseHandle(dds_shared_file);
    }

    std::cout << "\nPress enter key to exit." << std::endl;
    std::cin.get();
    return 0;
}