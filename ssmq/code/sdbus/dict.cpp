
#include "sdbus/dict.h"
#include "sdbus/variant.h"
#include "impl/dict_impl.h"

namespace sdbus{

FieldDef::FieldDef()
{
    Type = 0 ;
    Tag = 0 ;
}

FieldDef::~FieldDef()
{
    //
}

FieldDef::FieldDef(const FieldDef& def)
{
    Assign(def) ;
}

FieldDef::FieldDef(int type , uint32_t tag , const sdbus::string& name)
{
    Type = type ;
    Tag = tag ;
    Name = name ;
}

FieldDef& FieldDef::operator=(const FieldDef& def)
{
    Assign(def) ;
    return (*this) ;
}

bool FieldDef::operator==(const FieldDef& def) const
{
    return Equal(def) ;
}

bool FieldDef::operator!=(const FieldDef& def) const
{
    return !Equal(def) ;
}

FieldDef::operator uint32_t() const 
{
    return Tag ;
}


bool FieldDef::Validate() const
{
    if(Type != Variant::VAR_VOID && Tag > 0)
        return true ;
    else
        return false ;
}

void FieldDef::Assign(const FieldDef& def)
{
    Type = def.Type ;
    Tag = def.Tag ;
    Name = def.Name ;
}

bool FieldDef::Equal(const FieldDef& def) const
{
    if(Type == def.Type && Tag == def.Tag && Name == def.Name)
        return true ;
    else
        return false ;
}

const FieldDef FieldDef::EMPTY ;


static const int TYPE_DESC_MAX = 16 ;
static const char * type_desc[TYPE_DESC_MAX] = {

    "void" ,

    "bool" ,

    "int8" ,
    "uint8" ,
    "int16" ,
    "uint16" ,
    "int32" ,
    "uint32" ,
    "int64" ,
    "uint64" ,

    "float" ,
    "double" ,

    "string" ,
    "raw" ,

    "map" ,
    "list" 
} ;

int FieldDict::Desc2Type(const char * name)
{
    if(name == NULL)
        return 0 ;

    for(int idx = 0 ; idx < TYPE_DESC_MAX ; ++idx)
    {
        if(strcmp(name , type_desc[idx]) == 0)
            return idx ;
    }

    return 0 ;
}

const char * FieldDict::Type2Desc(int idx) 
{
    if(idx < 0 || idx > TYPE_DESC_MAX)
        return NULL ;
    else
        return type_desc[idx] ;
}

FieldDict& FieldDict::Singleton()
{
    static FieldDict __fieldict_singleton__ ;
    return __fieldict_singleton__ ;
}

FieldDict::FieldDict()
{
    impl_ = new FieldDictImpl() ;
}

FieldDict::~FieldDict()
{
    if(impl_ != NULL)
    {
        delete impl_ ;
        impl_ = NULL ;
    }
}


bool FieldDict::Add(const FieldDef& def)
{
    return impl_->Add(def) ;
}

bool FieldDict::Add(int type , uint32_t tag , const sdbus::string& name) 
{
    return impl_->Add(type , tag , name) ;
}

const FieldDef& FieldDict::Find(uint32_t tag) const
{

    return impl_->Find(tag) ;
}

const FieldDef& FieldDict::Find(const sdbus::string& name)const
{
    return impl_->Find(name) ;
}

const FieldDef& FieldDict::Find(const char * name)  const
{
    return impl_->Find(name) ;
}

int FieldDict::Size() const
{
    return impl_->Size() ;
}

const FieldDef& FieldDict::GetByIndex(int index) const
{
    return impl_->GetByIndex(index) ;
}

bool FieldDict::Empty() const 
{
    return impl_->Empty() ;
}

}

