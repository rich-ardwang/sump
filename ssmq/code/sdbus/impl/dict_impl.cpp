
#include "dict_impl.h"
#include "SSRTL/SSConfig.h"
#include "SSRTL/SSString.h"
#include "SSRTL/SSLog.h"
#include "sdbus/variant.h"
//#include "sdbus/transcoder.h"

#include <string>

namespace sdbus{


FieldDictImpl::FieldDictImpl()
{
    //
}

FieldDictImpl::~FieldDictImpl()
{
    //
}

bool FieldDictImpl::Add(const FieldDef& def)
{
    if(def.Validate() == false)
        return false ;

    int index = tags_.Set(def.Tag) ;
    if(index < 0)
        return false ;

    TagType::NodeType * node = tags_.GetNode(index) ;
    if(node == NULL)
        return false ;
    node->Value = def ;

    return names_.Set(def.Name , index) ;
}

bool FieldDictImpl::Add(int type , uint32_t tag , const sdbus::string& name)
{
    return Add(FieldDef(type , tag , name)) ;
}

const FieldDef& FieldDictImpl::Find(uint32_t tag) const
{
    const TagType::NodeType * node = tags_.FindNode(tag) ;
    if(node == NULL)
        return FieldDef::EMPTY ;
    return node->Value ;
}

const FieldDef& FieldDictImpl::Find(const sdbus::string& name) const
{
    const NameType::NodeType * node = names_.FindNode(name) ;
    if(node != NULL)
    {
        int tidx = node->Value ;
        const TagType::NodeType * tnode = tags_.GetNode(tidx) ;
        if(tnode != NULL)
        {
            return tnode->Value ;
        }
    }
    return FieldDef::EMPTY ;
}

int FieldDictImpl::Size() const
{
    return (int)tags_.Size() ;
}

const FieldDef& FieldDictImpl::GetByIndex(int index) const
{
    const TagType::NodeType * node = tags_.GetNode(index) ;
    if(node != NULL)
        return node->Value ;
    else
        return FieldDef::EMPTY ;
}


}
