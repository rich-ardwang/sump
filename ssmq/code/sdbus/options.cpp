
#include "sdbus/options.h"
#include "sdbus/variant.h"
#include "sdbus/string.h"
#include "SSRTL/SSString.h"

#include <map>

namespace sdbus{

class OptionsImpl : public std::map<sdbus::string , Variant>{ } ;

const Options Options::EMPTY ;

Options::Options()
{
    impl_ = new OptionsImpl() ;
}

Options::Options(const sdbus::string& options)
{
    impl_ = new OptionsImpl() ;
    Parse(options) ;
}

Options::Options(const Options& options)
{
    impl_ = new OptionsImpl() ;
    *impl_ = *(options.impl_) ;
}


Options::~Options()
{
    if(impl_ != NULL)
    {
        delete impl_ ;
        impl_ = NULL ;
    }
}

Options& Options::operator=(const Options& options)
{
    *impl_ = *options.impl_ ;
    return (*this) ;
}

Options& Options::operator=(const sdbus::string& options)
{
    Parse(options) ;
    return (*this) ;
}

bool Options::Parse(const sdbus::string& options) 
{
    impl_->clear() ;

    std::string pairs[256] ;
    int pcount = ss::string_split(options , pairs , ';') ;

    for(int pidx = 0 ; pidx < pcount ; ++pidx)
    {
        const std::string& pair = pairs[pidx] ;
        if(pair.empty() == true)
            continue ;

        std::string kv[32] ;
        int kvc = ss::string_split(pair , kv , ':') ;
        if(kvc != 2)
            continue ;

        SetOption(kv[0].c_str() , kv[1]) ;
    }

    return true ;
}

/*
    以key":"value";"格式，输入多个option
*/
bool Options::SetOptions(const sdbus::string& options)
{
    return Parse(options) ;
}

bool Options::SetOption(const sdbus::string& name , bool val)
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}

bool Options::SetOption(const sdbus::string& name , int32_t val)
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}

bool Options::SetOption(const sdbus::string& name , uint32_t val)
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}

bool Options::SetOption(const sdbus::string& name , int64_t val) 
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}

bool Options::SetOption(const sdbus::string& name , uint64_t val)
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}

bool Options::SetOption(const sdbus::string& name , float val)
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}

bool Options::SetOption(const sdbus::string& name , double val)
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}

bool Options::SetOption(const sdbus::string& name , const sdbus::string& val)
{
    return impl_->insert(std::pair<sdbus::string , Variant>(name , val)).second ;
}


bool Options::GetOption(const sdbus::string& name , bool& val) const 
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetBool(val) ;
}

bool Options::GetOption(const sdbus::string& name , int32_t& val) const
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetInt32(val) ;
}

bool Options::GetOption(const sdbus::string& name , uint32_t& val) const
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetUInt32(val) ;
}

bool Options::GetOption(const sdbus::string& name , int64_t& val) const
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetInt64(val) ;
}

bool Options::GetOption(const sdbus::string& name , uint64_t& val) const
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetUInt64(val) ;
}

bool Options::GetOption(const sdbus::string& name , float& val) const
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetFloat(val) ;
}

bool Options::GetOption(const sdbus::string& name , double& val) const
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetDouble(val) ;
}

bool Options::GetOption(const sdbus::string& name , sdbus::string& val) const 
{
    OptionsImpl::const_iterator citer = impl_->find(name) ;
    if(citer == impl_->end())
        return false ;

    const Variant& var = citer->second ;
    return var.GetString(val) ;
}


}
