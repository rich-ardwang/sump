//
//  sfa_container.h
//
//  Edited by lei.wang on 2019-08-26.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once 
#include <deque>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <mutex>
#include "sfa_base.h"

namespace sfa {
namespace utils {

template< class ValueT, class LockT = std::mutex >
class deque_lv : public std::deque<ValueT>,
	public LockT {};

template< class ValueT, class LockT = std::mutex >
class vector_lv : public std::vector<ValueT>,
	public LockT {};

template< class ValueT, class LockT = std::mutex >
class list_lv : public std::list<ValueT>,
	public LockT {};

template< class KeyT, class LockT = std::mutex >
class set_lv : public std::set< KeyT >,
	public LockT {};

template< class KeyT, class ValueT, class LockT = std::mutex >
class map_lv : public std::map< KeyT, ValueT >,
	public LockT {};

typedef base::scopelocker<std::mutex>		sfa_c_autolock;
typedef base::scopeunlocker<std::mutex>		sfa_c_autounlock;

}
}
