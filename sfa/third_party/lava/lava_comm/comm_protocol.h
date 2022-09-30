//
//  comm_protocol.h
//
//  Created by Colin on 2019-06-30.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once

namespace lava
{
	namespace comm
	{
		enum comm_protocol : uint8_t{ tcp_comm, udp_comm };
		enum proxy_type : uint8_t	{ proxy_none = 0, proxy_socks4 = 1, proxy_socks5 = 2, proxy_http = 3 };
		enum cluster_type : uint8_t { cluster_none = 0, cluster_zookeeper = 1 };

		struct client_proxy_info
		{
			proxy_type			m_proxy_type;
			const char*			m_proxy_host;
			const char*			m_proxy_port;
			const char*			m_proxy_username;
			const char*			m_proxy_password;
		};
		struct client_cluster_node_info
		{
			cluster_type		m_cluster_type;
			const char*			m_cluster_nodes_info;
			const char*			m_cluster_server_name;
		};
		struct client_login_info
		{
			bool						m_use_proxy;
			bool						m_use_cluster_mode;
			uint16_t					m_pad_reserved2;
			client_proxy_info*			m_proxy_info;
			const char*					m_host;
			const char*					m_port;
			const char*					m_username;
			const char*					m_password;
			
			client_cluster_node_info*	m_cluster_info;
		};
	}
}