
#include "msgid_manager.h"
#include "SSRTL/SSLog.h"

namespace sdbus{


MsgIDManager::MsgIDManager()
{
    final_ = false ;
}

MsgIDManager::~MsgIDManager()
{
    Final() ;
}

bool MsgIDManager::AddMsgID(uint32_t seq , const sdbus::string& msgid) 
{
    if(msgid.empty() == true)
        return true ;

    bool result = false ;

    idguard_.lock() ;

    result = msgids_.Set(seq , msgid) ;

    idguard_.unlock() ;

    if(result == false)
        return false ;

    toguard_.lock() ;

    time_t now = ::time(NULL) ;
    std::map<time_t , std::list<uint32_t> >::iterator titer = timeouts_.find(now) ;
    if(titer == timeouts_.end())
        titer = timeouts_.insert(std::pair<time_t , std::list<uint32_t> >(now , std::list<uint32_t>())).first ;

    std::list<uint32_t>& list = titer->second ;
    list.push_back(seq) ;

    toguard_.unlock() ;

    return true ;
}

const sdbus::string& MsgIDManager::FindMsgID(uint32_t seq) const
{
    boost::shared_lock<boost::shared_mutex> lock(idguard_) ;

    const MsgIDType::NodeType * node = msgids_.FindNode(seq) ;
    if(node != NULL)
        return node->Value ;

    
    return string::EMPTY ;
}

void MsgIDManager::CheckTimeout(int timeout) 
{
    if(final_ == true)
        return ;

    time_t now = ::time(NULL) ;
    time_t cur = 0 ;
    toguard_.lock_shared() ;

    for(std::map<time_t , std::list<uint32_t> >::const_iterator citer = timeouts_.begin() ; citer != timeouts_.end() ; ++citer)
    {
        time_t prev = citer->first ;
        int elaspe = (int)(now - prev) ;
        if(elaspe >= timeout)
        {
            cur = prev ;
            break ;
        }               
    }

    toguard_.unlock_shared() ;

    if(cur != 0)
    {
        //有超时的列表出现
        std::list<uint32_t> list ;
        toguard_.lock() ;
        std::map<time_t , std::list<uint32_t> >::iterator iter = timeouts_.find(cur) ;
        if(iter != timeouts_.end())
        {
            list = iter->second ;

            timeouts_.erase(iter) ;
        }
        toguard_.unlock() ;

        if(list.empty() == false)
        {
            //删除超时的包
            idguard_.lock() ;

            for(std::list<uint32_t>::const_iterator liter = list.begin() ; liter != list.end() ; ++liter)
            {
                uint32_t key = *liter ;
                if(msgids_.Del(key) == true)
                {
                    LOGNOTICE("we will delete request's msgid , sequence[%u] because of timeout[%d]" , *liter , timeout ) ;
                }
            }

            idguard_.unlock() ;
        }
    }//end if cur != 0
}

void MsgIDManager::Clear()
{
    toguard_.lock() ;
    timeouts_.clear() ;
    toguard_.unlock() ;

    idguard_.lock() ;
    msgids_.Clear() ;
    idguard_.unlock() ;
}

void MsgIDManager::Final()
{
    final_ = true ;
    Clear() ;
}

}
