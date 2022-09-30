//
//  sfa_base.h
//
//  Re-Edited by lei.wang on 2019-08-09.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#ifdef SFA_WIN
#include <Windows.h>
#endif
#include <sfa/sfa_macro.h>


namespace sfa {
namespace base {

#ifdef SFA_WIN
// end not contain '\'
static std::string getModulePath() {
	char buf[1024] = { 0 };
	if (::GetModuleFileNameA(NULL, buf, 1024) == 0) {
		return "";
	}
	if (::GetLongPathNameA(buf, buf, 1024) == 0) {
		return "";
	}

	char* end = strrchr(buf, '\\');
	buf[end - buf] = 0;
	std::string path = buf;
	std::string name = (end + 1);
	return path;
}
#endif

class noncopyable {
protected:
	noncopyable() {}
	~noncopyable() {}
	noncopyable(const noncopyable&) = delete;
	noncopyable& operator = (const noncopyable&) = delete;
};

template<class locker>
class scopelocker : public noncopyable {
public:
	explicit scopelocker(locker& l) : m_lock(l) {
		m_lock.lock();
	}
	~scopelocker() {
		m_lock.unlock();
	}

private:
	locker&		m_lock;
};

template<class locker>
class scopelocker_try : public noncopyable {
public:
	explicit scopelocker_try(locker& l) : m_lock(l), m_try_ok(false) {
		m_try_ok = m_lock.try_lock();
	}
	~scopelocker_try() {
		if (m_try_ok) {
			m_lock.unlock();
		}
	}
	bool try_ok() {
		return m_try_ok;
	}

private:
	locker&		m_lock;
	bool		m_try_ok;
};

template<class locker>
class scopeunlocker : public noncopyable {
public:
	explicit scopeunlocker(locker& l) : m_lock(l) {
		m_lock.unlock();
	}
	~scopeunlocker() {
		m_lock.lock();
	}

private:
	locker&		m_lock;
};

struct i_obj {
	virtual int32_t __stdcall reserved(void*, void**) = 0;
	virtual int32_t __stdcall add_ref() = 0;
	virtual int32_t __stdcall release() = 0;
};

template<typename i_plain_type>
class i_auto_ptr {
public:
	i_auto_ptr() : m_ptr(nullptr) {}
	template<typename derived_type>
	i_auto_ptr(derived_type* object) {
		m_ptr = static_cast<i_plain_type*>(object);
		if (m_ptr) {
			m_ptr->add_ref();
		}
	}
	i_auto_ptr(i_plain_type* ptr_) : m_ptr(ptr_) {
		if (m_ptr) {
			m_ptr->add_ref();
		}
	}
	i_auto_ptr(const i_auto_ptr& src_) : m_ptr(src_.m_ptr) {
		if (m_ptr) {
			m_ptr->add_ref();
		}
	}
	i_auto_ptr(i_auto_ptr&& src_) : m_ptr(src_.m_ptr) {
		src_.m_ptr = nullptr;
	}
	~i_auto_ptr() {
		if (m_ptr) {
			m_ptr->release();
		}
	}
	i_auto_ptr& operator = (const i_auto_ptr& rhs_) {
		if (this == rhs_) {
			return *this;
		}

		if (m_ptr) {
			m_ptr->release();
		}

		m_ptr = rhs_.m_ptr;
		if (m_ptr) {
			m_ptr->add_ref();
		}

		return *this;
	}
	i_auto_ptr& operator = (i_auto_ptr&& rhs_) {
		if (m_ptr) {
			m_ptr->release();
		}

		m_ptr = rhs_.m_ptr;
		rhs_.m_ptr = nullptr;

		return *this;
	}
	bool operator == (const i_auto_ptr& rhs_) const {
		return m_ptr == rhs_.m_ptr;
	}
	bool operator == (const i_plain_type* rhs_) const {
		return m_ptr == rhs_;
	}
	bool operator < (const i_auto_ptr& rhs_) const {
		return m_ptr < rhs_.m_ptr;
	}
	i_plain_type* operator -> () {
		return m_ptr;
	}
	operator bool() const {
		return m_ptr != nullptr;
	}
	void attach(i_plain_type* ptr_) {
		if (m_ptr) {
			m_ptr->release();
		}
		m_ptr = ptr_;
	}
	i_plain_type* detach() {
		i_plain_type* tmp = m_ptr;
		m_ptr = nullptr;
		return tmp;
	}
	i_plain_type* get() const {
		return m_ptr;
	}

private:
	i_plain_type*	m_ptr;
};

template<class T>
struct stack_object : public T {
	virtual int32_t __stdcall reserved(void*, void**){ return not_implemented; }
	virtual int32_t __stdcall add_ref() { return 0; /*nothing to do;*/ }
	virtual int32_t __stdcall release() { return 0; /*nothing to do;*/ }
};

#define i_obj_impl(count_type)	\
		public:\
		virtual int32_t __stdcall reserved(void*, void**)	\
		{	return not_implemented;	}		\
		virtual int32_t __stdcall add_ref()	\
		{	return ++m_ref_count;	}		\
		virtual int32_t __stdcall release()	\
		{	if (--m_ref_count == 0) { delete this; return 0; } return m_ref_count; }	\
		private:\
		count_type	m_ref_count = { 0 };

}
}
