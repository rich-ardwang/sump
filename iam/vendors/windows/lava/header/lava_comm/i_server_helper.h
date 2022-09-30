#pragma once
#include "lava_base.h"
#include "lava_utils_api.h"
namespace lava
{
	namespace comm
	{
		enum lava_service_type 
		{
			LST_UNKNOW,
			LST_ALL,			//全活模式
			LST_STANDBY,		//主备模式
			LST_BLANCE,			//负载均衡模式 应为BALANCE（之前错别字，为了兼容，先保持一致）
			LST_ROUNDROBIN		//消息轮询模式
		};
		struct i_server_info
			: public base::i_object
		{
			virtual void set_host(const char* host) = 0;
			virtual void set_port(int nPort) = 0;
			virtual void set_load_balance_factor(int factor) = 0;
			virtual void set_service_name(const char* name) = 0;
			// 外部请不要设置，cluster内部会维护和ZK的唯一id，后期更新如果不一致，会被cluster拒绝
			virtual void set_sequence_name(const char* seq_name) = 0;
			virtual void set_stat(int run) = 0;
			// 只有第一次注册时有效，后期更新会被覆盖，所以无法中途修改类型
			virtual void set_type(lava_service_type type) = 0;
			virtual void set_client_count(int count) = 0;
			virtual const char* get_sequence_name() = 0;
			virtual const char* get_host() = 0;
			virtual const char* get_service_name() = 0;
			virtual int get_port() = 0;
			virtual int get_load_balance_factor() = 0;
			virtual int get_type() = 0;
			virtual int get_stat() = 0;
			virtual int get_client_count() = 0;
		};

		struct i_server_helper_event_listener 
			: public base::i_object
		{
			enum
			{
				event_conn_connected,
				event_conn_disconnected,
				event_conn_closed,

				// for server
				event_register_server_success,
				event_unregister_server_success,
				event_update_server_data_success,

				// for query server list
				event_server_list_update,

				exception_conn_failed = 100,
				exception_register_server_failed,
				exception_unregister_server_failed,
				exception_query_server_list_failed,
				exception_update_server_data_failed,
			};
			virtual void on_event(int32_t event_code, void* event_data) = 0;
			// client event
			// work list is just reference, please check the state.
			virtual void on_server_list_update(const char * server_name, i_server_info** best_choice_servers, int  best_choice_count, i_server_info** work_servers, int work_count ,i_server_info** standby_servers, int standby_count) = 0;
			// server event
			// if your sequence name is in the work_servers list, please try to start your server or unregiste yourself
			virtual void on_cluster_node_change(i_server_info** best_choice_servers, int  best_choice_count, i_server_info** work_servers, int work_count, i_server_info** standby_servers, int standby_count) = 0;
			virtual void on_exception(int32_t error_code, const char* what) = 0;
		};

		class i_cluster_helper
		{
		public:
			virtual ~i_cluster_helper() {}
			virtual void interest(i_server_helper_event_listener* listener_) = 0;
			virtual bool register_server(__lv_inout lava::base::i_auto_ptr<i_server_info> server_info) = 0;
			virtual lava::base::i_auto_ptr<i_server_info> get_server_info() = 0;
			virtual bool update_server_data(__lv_inout lava::base::i_auto_ptr<i_server_info> server_info) = 0;
			virtual bool unregister_me() = 0;
			virtual bool watch_server_list_update(__lv_in const char* server_name) = 0;
			virtual bool unwatch_server_list_update(__lv_in const char* server_name) = 0;

			virtual lava::base::i_auto_ptr<lava::comm::i_server_info> choose_server_node(__lv_in const char* server_name, __lv_in int timeout=500/*milliseconds*/) = 0;
		};
	}
}

// cluster server helper
/*
// for zookeeper : addr:port;addr:port
*/
lava::comm::i_cluster_helper* create_cluster_helper(const char* cluster_server_param);
void destroy_cluster_helper(lava::comm::i_cluster_helper*);

lava::base::i_auto_ptr<lava::comm::i_server_info>  create_server_info(const char* server_name);