#ifndef __ZCONNECTION_H__
#define __ZCONNECTION_H__
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <list>


#include "zmq/zmq.h"
#include "sdbus/variant.h"
#include "sdbus/codec.h"
#include "sdbus/message.h"
#include "sdbus/callback.h"
#include "sdbus/connection.h"
#include "SSRTL/SSLog.h"
#include "notifier.h"
#include "engine/zmq_engine.h"

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/property_tree/ptree.hpp>
#include "boost/unordered_map.hpp"

#include <iostream>   
#include <sstream>
#include <algorithm>

#include <boost/uuid/uuid.hpp>   
#include <boost/uuid/uuid_generators.hpp>   
#include <boost/uuid/uuid_io.hpp>   
using namespace boost::uuids;  



#define ZMQ_HEAD_MSGTYPE_TAG      1
#define ZMQ_HEAD_MSGNAME_TAG      2
#define ZMQ_HEAD_MESSTYPE_TAG     3
#define ZMQ_HEAD_REPLYTO_TAG      4
#define ZMQ_HEAD_SUBJECT_TAG      5
#define ZMQ_HEAD_MESSID_TAG       6
#define ZMQ_HEAD_TOTAL_MSG_TAG    7
#define ZMQ_HEAD_IDX_MSG_TAG      8
#define ZMQ_HEAD_APPID_TAG        9


namespace engine{
class zmqframe
    {
    public: 
        static    const int bit_idf  = 0x1;
        static    const int bit_deli = 0x2;
        static    const int bit_head = 0x4;
        static    const int bit_content = 0x8;
        zmqframe(){  };
        ~zmqframe(){};

        static void zmqframe_auto_free( void *data,void *hint )
        {
            free(data);
        };
        void init()
        {
            zmq_msg_init(&identify);
            zmq_msg_init_data(&delimiter,"",0,NULL,NULL);
            zmq_msg_init(&head);
            zmq_msg_init(&content );
        }
        void close()
        {
            zmq_msg_close(&identify);
            zmq_msg_close(&delimiter);
            zmq_msg_close(&head);
            zmq_msg_close(&content );
            return;
        }


        int bitmask;
        int bit;
        zmq_msg_t identify;
        zmq_msg_t delimiter;
        zmq_msg_t head;
        zmq_msg_t content;
};
bool zmqframe_send(void *sock,zmqframe &zfm,int flag, int mask);
bool zmqframe_send_inner(void *sock,zmqframe &zfm,int flag, int mask);
namespace zmq{


#define HEAD_QUEUE_TYPE_FANOUT  sdbus::DeliverPublish
#define HEAD_QUEUE_TYPE_DIRECT  sdbus::DeliverDirect

#define HEAD_QUEUE_TYPE_SYSTEM_FANOUT  0xfd
#define HEAD_QUEUE_TYPE_SYSTEM_DIRECT  0xfe
#define HEAD_MAX_STRINE_LEN    64



bool zmqframe_to_sdbus(  zmqframe &zfm ,sdbus::Message &sdbus_msg,sdbus::string &msgname, uint8_t msgtype );
bool sdbus_to_zmqframe(const sdbus::Message& sdbus_msg , zmqframe &zfm ,sdbus::string name,int type);


#define SYNC_MSG_ID_PREFIX     "+sync+:"
#define SYNC_MSG_ID_PREFIX_LEN   7
class Zconnection{
public:
    static const int  _default_wmk = 1000000;

    static void gen_identify(sdbus::string &identify );
	static void gen_identify(sdbus::string &identify,char *prefix );
        Zconnection() ;
        Zconnection( sdbus::string &url );
        ~Zconnection();

        void Wait() {};
        void Final(){};

        bool Connect(const sdbus::string& url) ;
        void Close() ;
        sdbus::Callback *GetHandler( std::string name );
        bool SendSubCmd( const int type,const sdbus::string  &queue );
        bool AddHandler(sdbus::Callback * cb) ;

        void DelHandler(const sdbus::Callback * cb) ;

        bool AsyncSend(const sdbus::string& name , const sdbus::Message& msg , int type) ;
        bool SyncSend(const sdbus::string& name , const sdbus::Message& req , sdbus::Message& rsp , int timeout = -1) ;
        bool Publish(const sdbus::string& topic , const sdbus::Message& msg) ;

        bool SetOptions(const sdbus::Options& options) ;
        const sdbus::Options& GetOptions() const;
		bool CheckSyncHead( const sdbus::string &messgid );

private:
        void create_callback_thread();
        void do_recv_and_callback( );
		void en_do_recv_and_callback( );

private: 
        void* _zctx;
        void* _zconn;
        boost::thread * _callback_thread ;
        bool _closed;
        boost::mutex _sync_guard ;
        sdbus::string _url ;
        sdbus::string _identify;
        sdbus::Options _options ;
        int  _sendwmk;
        int  _recvwmk;
		int  _linger;
		NotifierManager notifiers_ ;
        boost::unordered_map< std::string ,sdbus::Callback * > _cbmap;
		ss::logger* m_pLogger;
  
} ;
}//end zero

namespace zbroker{

typedef struct _queue_info
{
   uint8_t  type;
   std::list< std::string > idf_list;
}queue_info;

typedef struct _store_queue
{
	size_t max;
	std::list< zmqframe* > list;
} store_queue;

typedef struct _client_info
{
    char *idf_str;
    int  idf_len;
    void *subsock;
    zmqframe zfm;
}client_info;

class zbroker
{
public:
	
    static const int  _default_wmk = 1000000;
	static const int  _default_max_store_msg = 1024;
    static int create( char *url,size_t len,size_t limit);
    static void do_recv( char *arg,size_t len,size_t limit );
	static ss::logger* GetLogger();
    zbroker(std::string &url );
	zbroker(std::string &url ,size_t limit);

    ~zbroker();
     int recv( zmqframe &zfm );
     int senddirect( std::list< std::string > &list, zmqframe &zfm,std::string &queue );
	 int _sendfanout( std::string &idf,zmqframe &zfm,std::string &queue );
	 int sendfanout( std::list< std::string > &list, zmqframe &zfm, std::string &queue);

     int add_sub_info(std::string &name,std::string &idf);
     queue_info *get_sub_info( std::string &name );
     int add_direct_info(std::string &name,std::string &idf);
     queue_info *get_direct_info( std::string &name );
	 int _store_msg( std::string &queue ,zmqframe &zmf );
	 int _req_store_msg( std::string &queue,std::string &idf );
	 int _send_store_msg( std::string &idf ,std::list< zmqframe* > &list );
	 int _senddirect( std::string &idf, zmqframe *zfm);
private:
    int _list_add( std::list< std::string > &list,std::string &idf );
	
private:
     bool _closed;
     sdbus::string _errmsg;
     sdbus::string _url;
     int  _sendwmk;
     int  _recvwmk;
	 int  _linger;
	 int  _storemsgnum;
     void *_ztx;
     void *_router;
     void *_exchange;
     boost::mutex _sync_guard ;
     //boost::unordered_map< std::string ,client_info * > _cimap;
     boost::unordered_map< std::string ,queue_info* > _submap;
     boost::unordered_map< std::string ,queue_info* > _queuemap;
	 boost::unordered_map< std::string ,store_queue* > _storemap;
};



}//end rbroker;



}//end egnine



#endif
