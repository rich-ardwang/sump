
#ifndef __SDBUS_MSGID_MANAGER_H
#define __SDBUS_MSGID_MANAGER_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include "sdbus/hash_map.h"

#include <map>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>


namespace sdbus{


class MsgIDManager{
public:
    MsgIDManager() ;
    ~MsgIDManager() ;

    bool AddMsgID(uint32_t seq , const sdbus::string& msgid) ;
    const sdbus::string& FindMsgID(uint32_t seq) const;

    void CheckTimeout(int timeout) ;
    void Clear() ;
    void Final() ;

private:
    typedef HashMap<uint32_t , sdbus::string> MsgIDType ;
    MsgIDType msgids_ ;

    mutable boost::shared_mutex idguard_ ;

    std::map<time_t , std::list<uint32_t> > timeouts_ ;
    mutable boost::shared_mutex toguard_ ;

    bool final_ ;
} ;

}

#endif  /** __SDBUS_MSGID_MANAGER_H */
