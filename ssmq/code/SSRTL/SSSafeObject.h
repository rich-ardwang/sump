
#ifndef __SS_SAFE_OBJECT_H
#define __SS_SAFE_OBJECT_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SSRTL/SSAtomic.h"

namespace ss{

class SafeObject : public atomic_count{
public:
    virtual ~SafeObject() {}
} ;

/*
    为了简化接口，SafeObjectRef直接定义SafeObject的子类，所以采用模板技术。
*/
template<typename T>
class SafeObjectRef{
public:
    SafeObjectRef() {object_ = NULL ;}
    SafeObjectRef(const SafeObjectRef& ref)
    {
        object_ = NULL ;
        Attach(ref.Get()) ;
    }

    SafeObjectRef(const SafeObject * obj)
    {
        object_ = NULL ;
        Attach((const T *)obj) ;
    }

    virtual ~SafeObjectRef() {Detach() ;}

    SafeObjectRef& operator=(const SafeObjectRef& ref)
    {
        Attach(ref.Get()) ;
        return (*this) ;
    }

    SafeObjectRef& operator=(const T * obj)
    {
        Attach(obj) ;
        return (*this) ;
    }

    bool operator==(const SafeObjectRef& ref) const
    {
        return (object_ == ref.Get()) ;
    }

    bool operator!=(const SafeObjectRef& ref) const
    {
        return (object_ != ref.Get()) ;
    }

    inline T * Get() const {return object_ ;}

    void Attach(const T * obj)
    {
        T * nobj = (T *)obj ;
        if(nobj != NULL)
            nobj->increment() ;

        Detach() ;
    
        object_ = nobj ;    
    }

    void Detach()
    {
        T * obj = object_ ;
        object_ = NULL ;

        if(obj != NULL)
        {
            long v = obj->decrement() ;
            if(v == 0)
                delete obj ;
        }    
    }

    inline T * operator->() const {return object_ ;}
    inline bool IsValid() const {return (object_ != NULL) ;}
    inline operator bool () const {return (object_ != NULL) ;}

private:
    mutable T * object_ ;
} ;


}

#endif  /** __SS_SAFE_OBJECT_H */
