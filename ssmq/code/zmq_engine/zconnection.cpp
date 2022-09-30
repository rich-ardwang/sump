#include "zconnection.h"
#include "..\..\inc\LogMarco.h"

#define DEFAULT_LINGER 200

namespace engine{

bool zmqframe_send(void *sock,zmqframe &zfm,int flag, int mask)
{
#ifdef BOOST_THREAD_WIN32
	DWORD dwExceptionCode(0);
	__try
	{
#endif

		return zmqframe_send_inner(sock, zfm, flag, mask);

#ifdef BOOST_THREAD_WIN32
	}
	__except(dwExceptionCode = GetExceptionCode())
	{
		return false;
	}
#endif
}

bool zmq_connect_with_exception(void *s, const char *addr)
{
#ifdef BOOST_THREAD_WIN32
    DWORD dwExceptionCode(0);
    __try
    {
#endif
        return 0 == zmq_connect(s, addr);

#ifdef BOOST_THREAD_WIN32
    }
    __except(dwExceptionCode = GetExceptionCode())
    {
        return false;
    }
#endif
}

bool zmqframe_send_inner(void *sock,zmqframe &zfm,int flag, int mask)
{
    int rc = -1;
    if(NULL == sock ) return false;
    if( mask & zmqframe::bit_idf )  
    {
        rc = zmq_msg_send( &zfm.identify,sock,flag|ZMQ_SNDMORE );
		if( rc < 0 ) return false;
        //printf("rc =%d\n",rc);
    }
    if( mask & zmqframe::bit_deli ) 
    {
        rc = zmq_msg_send( &zfm.delimiter, sock,flag|ZMQ_SNDMORE );
		if( rc < 0 ) return false;
        //printf("rc =%d\n",rc);
    }
    if( mask & zmqframe::bit_head ) 
    {
        rc = zmq_msg_send( &zfm.head, sock,flag|ZMQ_SNDMORE );
		if( rc < 0 ) return false;
        //printf("rc =%d\n",rc);
    }

    rc = zmq_msg_send( &zfm.content,sock,flag & (~ZMQ_SNDMORE));
	if( rc < 0 ) return false;
    //printf("rc =%d\n",rc);
    return true;

}
namespace zmq{
bool zmqframe_to_sdbus(  zmqframe &zfm ,sdbus::Message &sdbus_msg,sdbus::string &msgname, uint8_t msgtype )
{
    size_t headsize = 0;
    size_t contsize = 0;
    size_t used = 0 ;

    sdbus::string messid;
    sdbus::string messtype;
    sdbus::string replyto;
    sdbus::string subject;
    sdbus::uint32_t      total;
    sdbus::uint32_t      idx;


    sdbus::VariantMap  head;
    headsize = zmq_msg_size( &zfm.head );
    contsize = zmq_msg_size( &zfm.content );

    if(sdbus::VariantMapDecode(head , (char*) zmq_msg_data (&zfm.head) , headsize , used) == false)
        return false ;

    head.GetUInt8(ZMQ_HEAD_MSGTYPE_TAG,msgtype ) ;
    head.GetString(ZMQ_HEAD_MSGNAME_TAG,msgname );

    if( head.GetString( ZMQ_HEAD_MESSID_TAG,messid ) )
        sdbus_msg.SetMessageID( messid );
    if( head.GetString( ZMQ_HEAD_MESSTYPE_TAG,messtype ) )
        sdbus_msg.SetType( messtype );

    if( head.GetString(ZMQ_HEAD_SUBJECT_TAG,subject ) )
        sdbus_msg.SetSubject( subject );

    if( head.GetString(ZMQ_HEAD_REPLYTO_TAG ,replyto ) )
        sdbus_msg.SetReplyTo( replyto );

    if( head.GetUInt32( ZMQ_HEAD_TOTAL_MSG_TAG,total ) )
        sdbus_msg.SetTotalMessages( total );

    if( head.GetUInt32( ZMQ_HEAD_IDX_MSG_TAG,idx ))
        sdbus_msg.SetMessageNumber( idx );

    sdbus::Message::Map& values = sdbus_msg.MapValue() ;
    values.Clear() ;

    if(sdbus::VariantMapDecode(values , (char*) zmq_msg_data (&zfm.content ) , contsize , used) == false)
        return false ;
    return true;
}

bool sdbus_to_zmqframe(const sdbus::Message& sdbus_msg , zmqframe &zfm ,sdbus::string name,int type)
{
    const sdbus::VariantMap& values = sdbus_msg.MapValue() ;
    sdbus::VariantMap  head;

    head.SetUInt8(ZMQ_HEAD_MSGTYPE_TAG,type );
    head.SetString(ZMQ_HEAD_MSGNAME_TAG,name);

    if( NULL != sdbus_msg.GetSubject().c_str() )
        head.SetString(ZMQ_HEAD_SUBJECT_TAG, sdbus_msg.GetSubject().c_str() );

    if( NULL != sdbus_msg.GetMessageID().c_str() )
        head.SetString(ZMQ_HEAD_MESSID_TAG, sdbus_msg.GetMessageID().c_str());

    if( NULL != sdbus_msg.GetType().c_str() )
        head.SetString(ZMQ_HEAD_MESSTYPE_TAG,sdbus_msg.GetType().c_str());

    if( NULL != sdbus_msg.GetReplyTo().c_str())
        head.SetString(ZMQ_HEAD_REPLYTO_TAG, sdbus_msg.GetReplyTo().c_str() );

    if(sdbus_msg.GetTotalMessages() > 1)
    {
        head.SetUInt32( ZMQ_HEAD_TOTAL_MSG_TAG, sdbus_msg.GetTotalMessages() );
        head.SetUInt32( ZMQ_HEAD_IDX_MSG_TAG , sdbus_msg.GetMessageNumber() );
    }

    size_t headsize;
    if( sdbus::VariantMapCalcSize(head, headsize) == false )
    {
        return false;
    }

    size_t vsize = 0 ;
    if(sdbus::VariantMapCalcSize(values , vsize) == false)
        return false ;

    if( 0 != zmq_msg_init_size( &zfm.head,headsize))
    {
        return false;
    }

    if( 0 != zmq_msg_init_size(&zfm.content,vsize))
    {
        zmq_msg_close( &zfm.head );
        return false;
    }

    size_t used = 0 ;
    sdbus::VariantMapEncode(head , (char*) zmq_msg_data (&zfm.head) , headsize , used);
    sdbus::VariantMapEncode(values ,(char*) zmq_msg_data (&zfm.content) , vsize , used);

    return true ;
}


void Zconnection::gen_identify(sdbus::string &identify )
{
    boost::uuids::random_generator rgen;

    boost::uuids::uuid a_uuid = rgen();
    std::stringstream ss;
    ss << a_uuid;
    std::string str;
    ss >> str;
    printf("uuid:%s\n",str.c_str());
    identify = str.c_str();
    return;
}
void Zconnection::gen_identify(sdbus::string &identify,char *prefix )
{
	boost::uuids::random_generator rgen;

	boost::uuids::uuid a_uuid = rgen();
	std::stringstream ss;
	ss << a_uuid;
	std::string str;
	ss >> str;
	
	identify = prefix;
	identify += str;

	printf("uuid:%s\n",identify.c_str());
	return;
}
Zconnection::Zconnection() 
:m_pLogger(NULL)
,_linger(DEFAULT_LINGER)
{ 
	m_pLogger = new ss::logger("log\\ZCONN");
	m_pLogger->set_level_by_config("config\\logconfig.ini", "maincfg.zmqengine");

    _zctx  = NULL;
    _zconn = NULL;
	_callback_thread = NULL;
    _sendwmk = _recvwmk = _default_wmk;
    _zctx = zmq_ctx_new();
    if( NULL != _zctx )
    {
        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "zmq_ctx_new success!\n");
        _zconn = zmq_socket(_zctx ,ZMQ_DEALER );
    }
    else
    {
        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, "zmq_ctx_new failed!\n");
    }
}

Zconnection::Zconnection( sdbus::string &url )
:m_pLogger(NULL)
,_linger(DEFAULT_LINGER)
{
	m_pLogger = new ss::logger("log\\ZCONN");
	m_pLogger->set_level_by_config("config\\logconfig.ini", "maincfg.zmqengine");

    _zctx  = NULL;
    _zconn = NULL;
	_callback_thread = NULL;
    _sendwmk = _recvwmk = _default_wmk;
            
    _zctx = zmq_ctx_new();
    if( NULL != _zctx )
    {
        _zconn = zmq_socket(_zctx ,ZMQ_DEALER );
        if( NULL == _zconn)
        {
            zmq_ctx_destroy( _zctx );
            _zctx = NULL;
            return;
        }

        gen_identify( _identify );
        zmq_setsockopt(_zconn,ZMQ_IDENTITY,_identify.c_str() ,strlen( _identify.c_str() ) );
                
		zmq_setsockopt(_zconn,ZMQ_SNDHWM,&_sendwmk,sizeof(int));
		zmq_setsockopt(_zconn,ZMQ_RCVHWM,&_recvwmk,sizeof(int));

		zmq_setsockopt(_zconn,ZMQ_LINGER,&_linger,sizeof(int));
                
        if( !zmq_connect_with_exception( _zconn,url.c_str() ))
        {
            zmq_close( _zconn);
            zmq_ctx_destroy(_zctx);
            _zconn = NULL;
            _zctx  = NULL;
            return;
        }
        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "zmq_connect success!\n");
    }
    else
    {
        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, "zmq_ctx_new failed!\n");
    }
    _url = url;
	_closed = false;
        
};

bool env_zmq_close(void* s)
{
	if (!s)
	{
		return true;
	}
#ifdef BOOST_THREAD_WIN32
	DWORD dwExceptionCode(0);
	__try
	{
#endif

		zmq_close(s);

#ifdef BOOST_THREAD_WIN32
	}
	__except(dwExceptionCode = GetExceptionCode())
	{
		return false;
	}
#endif

	return true;
};

bool env_zmq_ctx_destroy(void* ctx)
{
	if (!ctx)
	{
		return true;
	}
#ifdef BOOST_THREAD_WIN32
	DWORD dwExceptionCode(0);
	__try
	{
#endif

		zmq_ctx_destroy(ctx);

#ifdef BOOST_THREAD_WIN32
	}
	__except(dwExceptionCode = GetExceptionCode())
	{
		return false;
	}
#endif

	return true;
}

Zconnection::~Zconnection()
{ 
	Close();
	if (NULL != _callback_thread)
	{
		delete _callback_thread;
		_callback_thread = NULL;
	}

	env_zmq_close(_zconn );
	env_zmq_ctx_destroy(_zctx);

	if (m_pLogger)
	{
		delete m_pLogger;
		m_pLogger = NULL;
	}
} ;

bool Zconnection::Connect(const sdbus::string& url) 
{   
    boost::mutex::scoped_lock lock(_sync_guard) ;

    if( _url == url ) return true;
    do 
    {
        if( NULL == _zctx ) _zctx = zmq_ctx_new();
        if( NULL == _zctx ) break;

        if( NULL == _zconn ) _zconn = zmq_socket( _zctx,ZMQ_DEALER );
        if( NULL == _zconn ) break;
        gen_identify( _identify );
        zmq_setsockopt(_zconn,ZMQ_IDENTITY,_identify.c_str() ,strlen( _identify.c_str() ) );
        zmq_setsockopt(_zconn,ZMQ_SNDHWM,&_sendwmk,sizeof(int));
		zmq_setsockopt(_zconn,ZMQ_RCVHWM,&_recvwmk,sizeof(int));
		zmq_setsockopt(_zconn,ZMQ_LINGER,&_linger,sizeof(int));
        if( !zmq_connect_with_exception( _zconn,url.c_str() ) ) break;

                 
        _url = url;
		_closed = false;
        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "connect to [%s] success!\n" ,_url.c_str());
        return true;
    } while (0);

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, "connect to [%s] failed!\n" ,_url.c_str());
    return false;
};
void Zconnection::Close() 
{
    boost::mutex::scoped_lock lock(_sync_guard) ;
    if( true == _closed )  return;
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "connection[%s]  be closing...\n" ,_url.c_str());
    _closed = true;

    //do wait;
	if (_callback_thread)
	{
		_callback_thread->join();
	}

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "connection[%s]  be closed\n" ,_url.c_str()) ;
    return;
}
sdbus::Callback* Zconnection::GetHandler( std::string name )
{
    boost::unordered_map< std::string ,sdbus::Callback* >::iterator it = _cbmap.find( name );
    if( it == _cbmap.end() )  return NULL;
    return (sdbus::Callback *)it->second;
}
bool Zconnection::SendSubCmd( const int type,const sdbus::string  &queue )
{
    zmqframe zfm;
    sdbus::VariantMap headmap;
    if( HEAD_QUEUE_TYPE_DIRECT == type )
        headmap.SetUInt8(ZMQ_HEAD_MSGTYPE_TAG,HEAD_QUEUE_TYPE_SYSTEM_DIRECT );
    else
        headmap.SetUInt8(ZMQ_HEAD_MSGTYPE_TAG,HEAD_QUEUE_TYPE_SYSTEM_FANOUT );

    headmap.SetString(ZMQ_HEAD_MSGNAME_TAG,queue );
    size_t headsize;
    if( sdbus::VariantMapCalcSize(headmap, headsize) == false )
    {
        return false;
    }
    if( 0 != zmq_msg_init_size( &zfm.head,headsize))
    {
        return false;
    }
    size_t used = 0 ;
    sdbus::VariantMapEncode(headmap, (char*) zmq_msg_data (&zfm.head) , headsize , used);

    zmq_msg_init_data( &zfm.content,"",0,NULL,NULL );

    if( ! zmqframe_send( _zconn,zfm,ZMQ_DONTWAIT,
        zmqframe::bit_head  ) )

    {
        zfm.close();
        return false;
    }
    return true;
}

bool Zconnection::AddHandler(sdbus::Callback * cb) 
{
            
    if( NULL == cb ) return false;

    const sdbus::string &name = cb->Name();
    const int &type =  cb->Type();
    const std::string stdname = name.c_str();
    if( sdbus::string::EMPTY == stdname ) return false;

    if( !SendSubCmd( type,name) )
    {
        return false;
    }

    _cbmap[ stdname ] = cb;

    create_callback_thread();
    return true;
}

void Zconnection::DelHandler(const sdbus::Callback * cb) 
{
    if( NULL == cb ) return;
    const sdbus::string &name = cb->Name();
    const std::string stdname = name.c_str();
    boost::unordered_map< std::string ,sdbus::Callback* >::iterator it = _cbmap.find( stdname );
    if( it != _cbmap.end() )
    {

        _cbmap.erase( it );
    }
    return;
}

bool Zconnection::AsyncSend(const sdbus::string& name , const sdbus::Message& msg , int type) 
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, " send:%s type:%d msgid:%s\n" , name.c_str(),type,msg.GetMessageID().c_str());
    zmqframe zfm;
    zfm.init();

    if ( !sdbus_to_zmqframe( msg,zfm,name,type ) )
	{
		CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, " send:%s type:%d msgid:%s trans failed!\n" , name.c_str(),type,msg.GetMessageID().c_str());
		return false;
	}

    boost::mutex::scoped_lock lock(_sync_guard) ;

    if( ! zmqframe_send( _zconn,zfm,0,
                            zmqframe::bit_head | zmqframe::bit_content ) )

    {
		CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, " send:%s type:%d msgid:%s send failed!\n" , name.c_str(),type,msg.GetMessageID().c_str());
        return false;
    }
    return true;
}
bool Zconnection::CheckSyncHead( const sdbus::string &messgid )
{
	size_t sz = messgid.length();
	const char *data = messgid.c_str();

	if( sz < SYNC_MSG_ID_PREFIX_LEN ) return false;
	int ret = ::memcmp(data,SYNC_MSG_ID_PREFIX,SYNC_MSG_ID_PREFIX_LEN);
	if(  0 != ret ) return false;
	return true;
}
bool Zconnection::SyncSend(const sdbus::string& name , const sdbus::Message& req , sdbus::Message& rsp , int timeout ) 
{
	sdbus::string rawid = req.GetMessageID();
	sdbus::string randid;

	gen_identify( randid, SYNC_MSG_ID_PREFIX );

	sdbus::Message &req_op = (sdbus::Message &)req;
	req_op.SetMessageID( randid  );

	bool ret = true;

	sdbus::Message *rspmsg = NULL;
	if( true == notifiers_.BeginNotifier( randid ) )
	{
		if( false == AsyncSend(name,req_op,HEAD_QUEUE_TYPE_DIRECT )  || NULL == (rspmsg = notifiers_.Wait(randid,timeout) ) )
		{
			ret = false;
		}
		if( NULL != rspmsg )
		{
			rsp = *rspmsg;
			rsp.SetMessageID( rawid );
			delete rspmsg;
		}
	}
	else
	{
		ret = false;
	}
	req_op.SetMessageID( rawid );
	notifiers_.RemoveNotifier( randid );
    if (!ret)
    {
        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, " SyncSend Failed ! randid:%s, rawid:%s \n" , randid.c_str(), rawid.c_str());
    }
    return ret;
}

 bool Zconnection::Publish(const sdbus::string& topic , const sdbus::Message& msg) 
{
    zmqframe zfm;
            

    if ( !sdbus_to_zmqframe( msg,zfm,topic,HEAD_QUEUE_TYPE_FANOUT ) )
        return false;

    boost::mutex::scoped_lock lock(_sync_guard) ;

    if( ! zmqframe_send( _zconn,zfm,ZMQ_DONTWAIT,
            zmqframe::bit_head | zmqframe::bit_content ) )

    {
        return false;
    }
    return true;
}
void Zconnection::create_callback_thread()
{
    boost::mutex::scoped_lock lock(_sync_guard) ;
    if(_callback_thread == NULL)
    {
        _callback_thread = new boost::thread(boost::bind(&Zconnection::en_do_recv_and_callback ,this)) ;   
    }    
    return;
}

void Zconnection::do_recv_and_callback( )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "we have started a recv thread\n");
    sdbus::Message sdmsg;
    sdbus::Message *psdmsg = &sdmsg;

	sdbus::Message *asyncmsg = NULL;
    zmqframe zfm;
    sdbus::string msgname;
            
    std::string   stdmsgname;
    uint8_t       msgtype = 255;
    sdbus::Callback *callback = NULL;
    int rc = -1;
	int rc1 = -1;
    int count = 0;
    
    while( ! _closed  )
    {
		sdmsg.Clear();
		zfm.init();

        rc = zmq_msg_recv( &zfm.head,_zconn,ZMQ_DONTWAIT );
		if (-1 == rc)
		{
			Sleep(50);
			zfm.close();
			continue;
		}
        rc1 = zmq_msg_recv( &zfm.content,_zconn,ZMQ_DONTWAIT );
		if (-1 == rc1)
		{
			Sleep(50);
			zfm.close();
			continue;
		}

        if( !zmqframe_to_sdbus(zfm,sdmsg,msgname,msgtype ) )
        {
            CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, "zmqframe translate to sdbus message failed");
			zfm.close();
            continue;
        }
		zfm.close();
        stdmsgname = msgname.c_str();
		sdbus::string mesgid = sdmsg.GetMessageID();
		if( !notifiers_.CheckMessageID( mesgid)  )
		{
            CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "recv async message response,messagename:%s,messageid:%s\n", msgname.c_str(), mesgid.c_str() );
			callback = GetHandler( stdmsgname );
			if( callback )
			{
				callback->OnMsgs( (const sdbus::Message **)&psdmsg,++count );
			}
		}
		else
		{
			CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "recv sync message response,messagename:%s,messageid:%s\n", msgname.c_str(), mesgid.c_str() );

			if( NULL == asyncmsg ) asyncmsg = new sdbus::Message;
			if( NULL != asyncmsg )
			{
				*asyncmsg = sdmsg;
				notifiers_.Notify(asyncmsg->GetMessageID() , asyncmsg);
				asyncmsg = NULL;
			}

		}
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "we have ended a recv thread\n");
    return;
}

void Zconnection::en_do_recv_and_callback( )
{
#ifdef BOOST_THREAD_WIN32
	DWORD dwExceptionCode(0);
	__try
	{
#endif

		do_recv_and_callback( );

#ifdef BOOST_THREAD_WIN32
	}
	__except(dwExceptionCode = GetExceptionCode())
	{
		CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_ERR, "zmq raise exception!\n");
	}
#endif
}

}//end zero

namespace zbroker{
int zbroker::create( char *url,size_t len,size_t limit)
{
    boost::thread *thd = new boost::thread(boost::bind(&zbroker::do_recv ,url,len,limit)) ; 
    if( NULL == thd ) return -1;
    return 0;
};
void zbroker::do_recv( char *arg,size_t len ,size_t limit )
{
    int rc = -1;
    size_t used = 0;
    size_t size_head;
    size_t size_idf;
    char   *cptr;
    std::string url = (char*)arg;
    std::string idf;
    std::string queue_name;
    uint8_t     queue_type;
    queue_info  *info = NULL;

    zbroker     broker( url ,limit);
    sdbus::VariantMap  head_map;
    zmqframe    zfm;
    while ( !broker._closed )
    {
        rc = broker.recv( zfm );
        if( 0 != rc ) continue;
        size_head = zmq_msg_size( &zfm.head );
        head_map.Clear();
        rc = sdbus::VariantMapDecode( head_map,(char*)zmq_msg_data(&zfm.head),size_head,used );
        head_map.GetString(ZMQ_HEAD_MSGNAME_TAG, queue_name );
        head_map.GetUInt8( ZMQ_HEAD_MSGTYPE_TAG, queue_type );
        size_idf = zmq_msg_size( &zfm.identify );
        cptr =(char *) zmq_msg_data( &zfm.identify );
        idf = std::string( cptr,size_idf );

        switch( queue_type )
        {
        case HEAD_QUEUE_TYPE_SYSTEM_FANOUT:
            broker.add_sub_info( queue_name,idf );
            break;
        case HEAD_QUEUE_TYPE_SYSTEM_DIRECT:
            broker.add_direct_info(queue_name,idf);
			broker._req_store_msg(queue_name,idf );
            break;
        case HEAD_QUEUE_TYPE_FANOUT:
            info = broker.get_sub_info( queue_name );
            if( NULL == info ) continue;
			broker.sendfanout( info->idf_list,zfm ,queue_name );
            break;
        case HEAD_QUEUE_TYPE_DIRECT:
            info = broker.get_direct_info( queue_name );
			if(  NULL == info || info->idf_list.empty() )
			{
				CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_WARN, "no client subscribe queue[%s],store to mem\n", queue_name.c_str() );
				broker._store_msg(queue_name,zfm);
			}
			else
			{
				broker.senddirect( info->idf_list,zfm ,queue_name );
			}
            break;
        default:
            break;

        }
    }
}

ss::logger* zbroker::GetLogger()
{
	static ss::logger sRet("log\\ZBROKER");
	sRet.set_level_by_config("config\\logconfig.ini", "maincfg.zmqengine");
	return &sRet;
}

zbroker::zbroker(std::string &url )
:_linger(DEFAULT_LINGER)
{
    int rc;
    _url = url;
    _ztx = zmq_ctx_new();
	_storemsgnum = zbroker::_default_max_store_msg;
	_sendwmk = _recvwmk = _default_wmk;

    if( _ztx )
    {
        _router = zmq_socket(_ztx,ZMQ_ROUTER );
        if( !_router ) 
        {
            _errmsg = "create zmq socket failed";
            _closed = true;
        }

		zmq_setsockopt(_router,ZMQ_SNDHWM,&_sendwmk,sizeof(int));
		zmq_setsockopt(_router,ZMQ_RCVHWM,&_recvwmk,sizeof(int));
		zmq_setsockopt(_router,ZMQ_LINGER,&_linger,sizeof(int));

        rc = zmq_bind( _router, _url.c_str() );
        if( 0 == rc ) 
        {
            _errmsg = "bind failed";
            _closed = true;
        }
        //_exchange = zmq_socket(_ztx,ZMQ_PUB );
    }
    _closed = false;
}

zbroker::zbroker(std::string &url,size_t limit )
:_linger(DEFAULT_LINGER)
{
	int rc;
	_url = url;
	_ztx = zmq_ctx_new();

	if( limit > zbroker::_default_max_store_msg)
		_storemsgnum = zbroker::_default_max_store_msg;
	else
		_storemsgnum = limit;
	_sendwmk = _recvwmk = _default_wmk;

	if( _ztx )
	{
		_router = zmq_socket(_ztx,ZMQ_ROUTER );
		if( !_router ) 
		{
			_errmsg = "create zmq socket failed";
			_closed = true;
		}

		zmq_setsockopt(_router,ZMQ_SNDHWM,&_sendwmk,sizeof(int));
		zmq_setsockopt(_router,ZMQ_RCVHWM,&_recvwmk,sizeof(int));
		zmq_setsockopt(_router,ZMQ_LINGER,&_linger,sizeof(int));

		rc = zmq_bind( _router, _url.c_str() );
		if( 0 == rc ) 
		{
			_errmsg = "bind failed";
			_closed = true;
		}
		//_exchange = zmq_socket(_ztx,ZMQ_PUB );
	}
	_closed = false;
}



zbroker::~zbroker()
{
    zmq_close( _router );
    zmq_close( _exchange );
    zmq_ctx_destroy( _ztx );
}

int zbroker::recv( zmqframe &zfm )
{

    int rc = 0;
    zfm.init();
         
    do 
    {
        rc = zmq_msg_recv( &zfm.identify,_router,0);

        rc = zmq_msg_recv( &zfm.head , _router,0);

        rc = zmq_msg_recv( &zfm.content,_router,0);

        return 0;
    } while ( 0 );
    return -1;
}


int zbroker::senddirect( std::list< std::string > &list, zmqframe &zfm,std::string &queue )
{
	std::string idf;
	const char *tmp = NULL;
	size_t sz= 0;
	int rc;
	while( !list.empty() )
	{
		idf = list.front();
		tmp = (char*)idf.c_str();
		sz = idf.length();
		rc = zmq_router_check_identify( _router,(char*)tmp,sz );
		//printf("rc = %d\n",rc);
		if ( 0 == rc)
		{
			break;
		}
		else
		{
			list.pop_front();
			idf.clear();
		}
	}
		 
	if( idf.empty() )
	{
		//buffer
		CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_WARN, "no client subscribe queue[%s]\n", queue.c_str() );
		zfm.close();
		return 0;
	}
	char *data = NULL;
	zmq_msg_close(&zfm.identify );
    zmq_msg_init_size(&zfm.identify, idf.length() );
	data = (char *)zmq_msg_data( &zfm.identify );
	if( NULL == data )
	{
		zfm.close();
		return -1;
	}
	tmp = idf.c_str();
	sz= idf.length();
	memcpy( data,tmp,sz );
	CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_INFO, " sendto:%s queue:%s " , tmp , queue.c_str());
	if( ! zmqframe_send( _router,zfm,ZMQ_DONTWAIT, zmqframe::bit_idf |zmqframe::bit_head | zmqframe::bit_content ) )
	{
		CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_ERR, " sendto:%s queue:%s failed!" , tmp , queue.c_str());
		zfm.close();
		return -1;
	}
	list.pop_front();
	list.push_back( idf );
    return 0;
}
int zbroker::_senddirect( std::string &idf, zmqframe *zfm)
{
	const char *stridf;
	char *data;
	size_t sz;
	int     rc;

	stridf = idf.c_str();
	sz = idf.length();
	rc = zmq_router_check_identify( _router,(char*)stridf,sz );
	if( 0 != rc ) return rc;

	data = (char *)zmq_msg_data( &zfm->identify );
	if( NULL == data )
	{
		return -1;
	}

	memcpy( data,stridf,sz );
	if( ! zmqframe_send( _router,*zfm,ZMQ_DONTWAIT, zmqframe::bit_idf |zmqframe::bit_head | zmqframe::bit_content ) )
	{
		return -1;
	}
	return 0;
}
int zbroker::_sendfanout( std::string &idf,zmqframe &zfm,std::string &queue )
{
	const char *stridf;
	size_t sz;
	int    rc;
	zmqframe tmp;
	char *data = NULL;

	stridf = idf.c_str();
	sz= idf.length();

	rc = zmq_router_check_identify( _router,(char*)stridf,sz );
	if( 0 != rc )
	{
		return -2;
	}

	tmp.init();
	zmq_msg_copy( &tmp.head, &zfm.head );
	zmq_msg_copy( &tmp.content, &zfm.content );
	zmq_msg_init_size(&tmp.identify, idf.length() );	
	data = (char *)zmq_msg_data( &tmp.identify );
	if( NULL == data )
	{
		tmp.close();
		return -1;
	}

	memcpy( data,stridf,sz );
	if( ! zmqframe_send( _router,tmp,ZMQ_DONTWAIT, zmqframe::bit_idf |zmqframe::bit_head | zmqframe::bit_content ) )
	{
		tmp.close();
		return -1;
	}
	return 0;
}

int zbroker::sendfanout( std::list< std::string > &list, zmqframe &zfm, std::string &queue)
{
	int rc = -1;
	for (std::list< std::string >::iterator it=list.begin(); it != list.end(); )
	{
		CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_INFO, " sendfanoutto:%s queue:%s \n" , it->c_str() , queue.c_str());
		rc =  _sendfanout(*it,zfm,queue );
		if( -2 == rc )
		{
			it = list.erase(it);
		}
		else
			it++;
	}
	zfm.close();
	return 0;
}
int zbroker::add_sub_info(std::string &name,std::string &idf)
{
    queue_info *info = get_sub_info( name );
    if( NULL == info  )
    {
        info = new queue_info;
        if( NULL == info ) 
        {
            _errmsg = "malloc queue info struct failed";
            return -1;
        }
        info->type = HEAD_QUEUE_TYPE_FANOUT;
        _submap[ name ] = info;
    }
    _list_add( info->idf_list ,idf );
    return 0;
}

queue_info *zbroker::get_sub_info( std::string &name )
{
    boost::unordered_map< std::string ,queue_info* >::iterator it = _submap.find( name );
    if( it == _submap.end() ) return NULL;
    return (queue_info *)it->second;

}

int zbroker::add_direct_info(std::string &name,std::string &idf)
{
    queue_info *info = get_direct_info( name );
    if( NULL == info  )
    {
        info = new queue_info;
        if( NULL == info ) 
        {
            _errmsg = "malloc queue info struct failed";
            return -1;
        }
        info->type = HEAD_QUEUE_TYPE_DIRECT;
        _queuemap[ name ] = info;
    }
    _list_add( info->idf_list ,idf );
    return 0;
}

queue_info* zbroker::get_direct_info( std::string &name )
{
    boost::unordered_map< std::string ,queue_info* >::iterator it = _queuemap.find( name );
    if( it == _queuemap.end() ) return NULL;
    return (queue_info *)it->second;
}


int zbroker::_list_add( std::list< std::string > &list,std::string &idf )
{
    std::list< std::string >::iterator it =find( list.begin(),list.end(),idf );
    if( it == list.end() ) list.push_back( idf );
	int i = list.size();
    return 0;
}
int zbroker::_send_store_msg( std::string &idf ,std::list< zmqframe* > &list )
{
	int rc = -1;
	zmqframe *pzmq = NULL;
	for (std::list< zmqframe* >::iterator it=list.begin(); it != list.end(); ++it )
	{
		pzmq = *it;
		if( NULL == pzmq ) continue;
		rc = _senddirect( idf,pzmq);
		if( 0 != rc )
		{
			CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_ERR, "send store msg to client[%s] failed,rc\n",idf.c_str(),rc);
			pzmq->close();
		}
		else
		{
			CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_INFO, "send store msg to client[%s] succ,rc =%d\n",idf.c_str(),rc);
		}
		delete pzmq;
	}
	list.clear();
	return 0;
}
int zbroker::_req_store_msg( std::string &queue,std::string &idf )
{
	store_queue *pqueue = NULL;
	zmqframe    *zmftmp = NULL;
	size_t      sz = 0;
	int         rc = 0;
	boost::unordered_map< std::string,store_queue* >::iterator it = _storemap.find( queue );
	if( it == _storemap.end() ) return 0;

	pqueue = (store_queue *)it->second;
	if( NULL == pqueue) return -1;
	rc =  _send_store_msg( idf, pqueue->list );
	_storemap.erase( it );
	delete pqueue;
	return rc;
}
int zbroker::_store_msg( std::string &queue ,zmqframe &zmf )
{
	store_queue *pqueue = NULL;
	zmqframe    *zmftmp = NULL;
	size_t      sz = 0;
	boost::unordered_map< std::string,store_queue* >::iterator it = _storemap.find( queue );
	if( it == _storemap.end() )
	{
		pqueue = new store_queue;
		if( NULL == pqueue )
		{
			CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_ERR, "%s:malloc store_queue failed\n",__FUNCTION__ );
			return -1;
		}
		pqueue->max = _storemsgnum;
		_storemap[ queue ] = pqueue;
	}
	else
	{
		pqueue = it->second;
	}
	
	if( NULL == pqueue ) 
	{
		zmf.close();
		return -2;
	}
	sz = pqueue->list.size();
	if( sz > pqueue->max )
	{
		CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_WARN, "%s:store msg too large %d\n",__FUNCTION__,sz );
		zmf.close();
		return -2;
	}
	zmftmp = new zmqframe;
	if( NULL == zmftmp )
	{
		CURLOGGERWRITEWITHLV(GetLogger(), ss::LOG_LEVEL::LOG_ERR, "%s:malloc zmqframe failed\n",__FUNCTION__ );
		zmf.close();
		return -1;
	}

	zmftmp->init();
	zmq_msg_copy(&zmftmp->identify,&zmf.identify );
	zmq_msg_copy(&zmftmp->head,&zmf.head);
	zmq_msg_copy(&zmftmp->content,&zmf.content);

	pqueue->list.push_back( zmftmp );
	zmf.close();
	return 0;
}

}//end zbroker


}//end engine