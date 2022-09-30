
#ifndef __SDBUS_CALLBACK_IMPL_H
#define __SDBUS_CALLBACK_IMPL_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread.hpp>

namespace sdbus{

class CallbackImpl{
public:
    CallbackImpl() ;
    virtual ~CallbackImpl() ;

    const sdbus::string& Name() const ;
    void Name(const char * name) ;

    inline int Type() const {return type_ ;}
    inline void Type(int type) {type_ = type ;}

    inline bool Durable() const {return durable_ ;}
    inline void Durable(bool durable) {durable_ = durable ;}

private:
    sdbus::string name_ ;
    int type_ ;
    bool durable_ ;

    mutable boost::shared_mutex guard_ ;
} ;

}

#endif  /**__SDBUS_CALLBACK_IMPL_H */
