
#ifndef __SDBUS_DICT_IMPL_H
#define __SDBUS_DICT_IMPL_H 1

#include "sdbus/dict.h"
#include "sdbus/hash_map.h"

#include <vector>
#include <map>

namespace sdbus{

class FieldDictImpl{
public:
    FieldDictImpl() ;
    ~FieldDictImpl() ;

    bool Add(const FieldDef& def) ;
    bool Add(int type , uint32_t tag , const sdbus::string& name) ;

    const FieldDef& Find(uint32_t tag) const ;
    const FieldDef& Find(const sdbus::string& name) const ;

    int Size() const ;
    const FieldDef& GetByIndex(int index) const ;

    inline bool Empty() const {return tags_.Size() == 0 ;}

private:
    typedef HashMap<uint32_t , FieldDef> TagType ;
    typedef HashMap<sdbus::string , int> NameType ;

    TagType tags_ ;
    NameType names_ ;
} ;


}

#endif  /** __SDBUS_DICT_IMPL_H */
