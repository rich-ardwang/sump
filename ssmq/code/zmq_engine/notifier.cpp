
#include "notifier.h"
#include <boost/thread/xtime.hpp>

namespace engine{
namespace zmq{

Notifier::Notifier()
{
    message_ = NULL ;
}

Notifier::~Notifier()
{
    boost::mutex::scoped_lock lock(guard_) ;
    if(message_ != NULL)
    {
        delete message_ ;
        message_ = NULL ;
    }
}

sdbus::Message * Notifier::Wait(int timeout) 
{
    boost::mutex::scoped_lock lock(guard_) ;

    if(timeout < 0)
    {
        notifier_.wait(lock) ;
    }
    else if(timeout == 0)
    {
        //
    }
    else{
        boost::xtime xt ;
        boost::xtime_get(&xt , boost::TIME_UTC_) ;
        xt.sec += timeout ;
        notifier_.timed_wait(lock , xt) ;
    }

    sdbus::Message * msg = message_ ;
    message_ = NULL ;

    return msg ;
}

void Notifier::Notify(sdbus::Message * message) 
{
    boost::mutex::scoped_lock lock(guard_) ;
    message_ = message;
    notifier_.notify_one() ;
}

NotifierManager::NotifierManager()
{
    //
}

NotifierManager::~NotifierManager()
{
    //
}

bool NotifierManager::BeginNotifier(const sdbus::string& id)
{
    if(id.empty() == true)
        return false ;

    boost::unique_lock<boost::shared_mutex> lock(guard_) ;
    std::map<sdbus::string , NotifierType>::iterator iter = notifiers_.find(id) ;
    if(iter != notifiers_.end())
        return false ;

    NotifierType notifier(new Notifier()) ;

    return notifiers_.insert(std::pair<sdbus::string , NotifierType>(id , notifier)).second ;
}

void NotifierManager::RemoveNotifier(const sdbus::string& id) 
{
    guard_.lock() ;
    std::map<sdbus::string , NotifierType>::iterator iter = notifiers_.find(id) ;
    if(iter != notifiers_.end())
        notifiers_.erase(iter) ;
    guard_.unlock() ;
}

sdbus::Message * NotifierManager::Wait(const sdbus::string& id , int timeout) 
{
    if(id.empty() == true)
        return NULL ;

    NotifierType notifier ;
    guard_.lock() ;
    std::map<sdbus::string , NotifierType>::iterator iter = notifiers_.find(id) ;
    if(iter != notifiers_.end())
        notifier = iter->second ;
    guard_.unlock() ;

    if(notifier.IsValid() == false)
        return NULL ;

    sdbus::Message * result = notifier->Wait(timeout) ;

    /*
        已经等待结束，要删除管理器里面的内容。
        不管什么原因，都不能在管理器里面遗留。
        有可能不会有回复的。
    */
    RemoveNotifier(id) ;
    return result ;
}

bool NotifierManager::Notify(const sdbus::string& id , sdbus::Message * msg)
{
    if(id.empty() == true)
        return false ;

    NotifierType notifier ;
    guard_.lock() ;
    std::map<sdbus::string , NotifierType>::iterator iter = notifiers_.find(id) ;
    if(iter != notifiers_.end())
    {
        notifier = iter->second ;
        notifiers_.erase(iter) ;
    }
    guard_.unlock() ;

    if(notifier.IsValid() == false)
        return false ;

    notifier->Notify(msg) ;
    return true ;
}

bool NotifierManager::CheckMessageID(const sdbus::string& id) const 
{
    boost::shared_lock<boost::shared_mutex> lock(guard_) ;
    if(notifiers_.empty())
        return false ;

    std::map<sdbus::string , NotifierType>::const_iterator citer = notifiers_.find(id) ;
    if(citer != notifiers_.end())
        return true ;
    else
        return false ;
}

}
}
