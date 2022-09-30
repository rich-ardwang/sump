/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : busmap.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-15
  *Desc        : 基于STL标准Map不是线程安全的，现将其实现为线程安全类型，以方便使用
  *History     : Add by Lei.Wang 2019-05-15
***************************************************************************************************/
#pragma once
#ifndef __BUS_MAP_H__
#define __BUS_MAP_H__
#include <map>
#include <Windows.h>
#define BusMap_CLock_Mutex_t                 HANDLE
#define BusMap_CLock_Mutex_Init(_mutex)      (_mutex = CreateSemaphore(NULL, 1, 1, NULL))
#define BusMap_CLock_Mutex_Lock(_mutex)      (WaitForSingleObject(_mutex, INFINITE))
#define BusMap_CLock_Mutex_UnLock(_mutex)    (ReleaseSemaphore(_mutex, 1, NULL))
#define BusMap_CLock_Mutex_Destroy(_mutex)   (CloseHandle(_mutex))
#define BusMap_Declar_Typename               typename


namespace busmap
{
    template<class K, class V>
    class map;

    template<class K, class V>
    class busmapIterator;

    //lock
    class CLock
    {
    public:
        CLock() { BusMap_CLock_Mutex_Init(_mutex); }
        ~CLock() { BusMap_CLock_Mutex_Destroy(_mutex); }

        void Lock() { BusMap_CLock_Mutex_Lock(_mutex); }
        void UnLock() { BusMap_CLock_Mutex_UnLock(_mutex); }

    private:
        BusMap_CLock_Mutex_t _mutex;
    };

    //threadlockstats
    /************************************************************************************/
    /*
        1.CThreadLockStats是保存了当前所有线程调用_mutex_usr.Lock()和_mutex_usr.UnLock()
        的差值,记录同一线程中_mutex_usr的使用情况。
        2.类中可用函数为TryLock(),TryUnLock()，如果同一线程中_mutex_usr已经Lock，则TryLock()
        不再锁，只是将标识自增；如果同一线程中_mutex_usr的Lock次数为1同时需要释放锁的时候，
        调用TryUnLock会释放，不为1则将标识自减。
    */
    /************************************************************************************/
    template<class K, class V>
    class CThreadLockStats
    {
    private:
        CThreadLockStats();
        ~CThreadLockStats();

        //防止重锁
        void TryLock();
        //与TryLock对应的释放锁
        void TryUnLock();
        //获取线程ID内联函数
        inline unsigned int PthreadSelf() {
            return GetCurrentThreadId();
        }

    private:
        CLock _mutex_usr;
        CLock _mutex_stats;
        std::map<unsigned int, int> *_thread_lock_stats;
        friend busmap::map<K, V>;
        friend busmap::busmapIterator<K, V>;
    };

    template<class K, class V>
    void busmap::CThreadLockStats<K, V>::TryUnLock()
    {
        bool _isneedusrunlock = false;
        unsigned int _thread_id = PthreadSelf();

        _mutex_stats.Lock();
        if (_thread_lock_stats) {
            std::map<unsigned int, int>::iterator _finditr = _thread_lock_stats->find(_thread_id);
            if (_finditr != _thread_lock_stats->end()) {
                if (1 == _finditr->second) {
                    _isneedusrunlock = true;
                }
                _finditr->second -= 1;
            }
            else {
                int _new_lock_counts = 0;
                _thread_lock_stats->insert(std::map<unsigned int, int>::value_type(_thread_id, _new_lock_counts));
            }
        }
        _mutex_stats.UnLock();

        if (_isneedusrunlock) {
            _mutex_usr.UnLock();
        }
    }

    template<class K, class V>
    void busmap::CThreadLockStats<K, V>::TryLock()
    {
        bool _isneedusrlock = false;
        unsigned int _thread_id = PthreadSelf();

        _mutex_stats.Lock();
        if (_thread_lock_stats) {
            std::map<unsigned int, int>::iterator _finditr = _thread_lock_stats->find(_thread_id);
            if (_finditr != _thread_lock_stats->end()) {
                if (_finditr->second <= 0) {
                    _finditr->second = 0;
                    _isneedusrlock = true;
                }
                _finditr->second += 1;
            }
            else {
                int _new_lock_counts = 1;
                _thread_lock_stats->insert(std::map<unsigned int, int>::value_type(_thread_id, _new_lock_counts));
                _isneedusrlock = true;
            }
        }
        _mutex_stats.UnLock();

        //将_mutex_usr.Lock放置于_mutex_stats释放后，防止死锁
        //函数中非成对出现的锁加上其他锁在多线程调用时一定会出现死锁
        if (_isneedusrlock) {
            _mutex_usr.Lock();
        }
    }

    template<class K, class V>
    busmap::CThreadLockStats<K, V>::~CThreadLockStats()
    {
        _mutex_stats.Lock();
        delete _thread_lock_stats;
        _thread_lock_stats = NULL;
        _mutex_stats.UnLock();
    }

    template<class K, class V>
    busmap::CThreadLockStats<K, V>::CThreadLockStats()
    {
        _thread_lock_stats = new std::map<unsigned int, int>;
    }

    //busmapIterator:迭代器继承标准迭代器,减少运算符和一些函数的重载
    template<class K, class V>
    class busmapIterator : public std::map<K, V>::iterator
    {
    public:
        busmapIterator();

        //重载拷贝构造，方便拷贝时就Lock,释放时就UnLock；
        busmapIterator(const busmapIterator &val_);
        ~busmapIterator();

        //赋值运算会隐藏父类赋值函数，不会继承，需要重载
        busmapIterator &operator=(const busmapIterator &val_);

    private:
        busmapIterator &operator=(const BusMap_Declar_Typename std::map<K, V>::iterator &val_);

    private:
        CThreadLockStats<K, V>* _mutex_stats;
        friend busmap::map<K, V>;
    };

    template<class K, class V>
    busmap::busmapIterator<K, V> &busmap::busmapIterator<K, V>::operator=(const busmap::busmapIterator<K, V> &val_)
    {
        _mutex_stats = val_._mutex_stats;
        if (_mutex_stats) {
            _mutex_stats->TryLock();
        }
        this->std::map<K, V>::iterator::operator=(val_);
        return *this;
    }

    template<class K, class V>
    busmap::busmapIterator<K, V>::busmapIterator(const busmapIterator &val_)
    {
        _mutex_stats = val_._mutex_stats;
        if (_mutex_stats) {
            _mutex_stats->TryLock();
        }
        this->std::map<K, V>::iterator::operator=(val_);
    }

    template<class K, class V>
    busmap::busmapIterator<K, V> &busmap::busmapIterator<K, V>::operator=(const BusMap_Declar_Typename std::map<K, V>::iterator &val_)
    {
        this->std::map<K, V>::iterator::operator=(val_);
        return *this;
    }

    //迭代器赋值和拷贝构造时Lock，析构时需要释放锁
    template<class K, class V>
    busmap::busmapIterator<K, V>::~busmapIterator()
    {
        if (_mutex_stats) {
            _mutex_stats->TryUnLock();
        }
        _mutex_stats = NULL;
    }

    template<class K, class V>
    busmap::busmapIterator<K, V>::busmapIterator()
    {
        _mutex_stats = NULL;
    }

    //busmap
    /**********************************************************************************/
    /*
        由于iterator默认构造函数没有TryLock，所以在返回该类型临时变量时需要TryLock一次，
        临时变量析构时TryUnLock一次；将返回的值进行拷贝构造或者拷贝赋值会TryLock一次，使用
        完后析构TryUnLock一次。达到成对锁和释放锁的目的。
    */
    /**********************************************************************************/
    template<class K, class V>
    class map : private std::map<K, V>
    {
    public:
        map();
        map(const map &val_);
        ~map();
        map &operator=(const map &val_);
        typedef BusMap_Declar_Typename busmapIterator<K, V> iterator;

        void insert_s(const K &key_, const V &val_);
        void erase_s(const K &key_);
        void erase_s(iterator &itr_);
        void clear_s();
        iterator find_s(const K &key_);
        iterator begin_s();
        iterator end_s();
        unsigned int size_s();

    private:
        CThreadLockStats<K, V> _mutex_stats;
    };

    template<class K, class V>
    unsigned int busmap::map<K, V>::size_s()
    {
        unsigned int _size = 0;
        _mutex_stats.TryLock();
        _size = this->size();
        _mutex_stats.TryUnLock();
        return _size;
    }

    template<class K, class V>
    BusMap_Declar_Typename busmap::map<K, V>::iterator busmap::map<K, V>::end_s()
    {
        busmap::map<K, V>::iterator _ret;
        _ret._mutex_stats = &_mutex_stats;
        _mutex_stats.TryLock();
        _ret = this->end();
        return _ret;
    }

    template<class K, class V>
    BusMap_Declar_Typename busmap::map<K, V>::iterator busmap::map<K, V>::begin_s()
    {
        busmap::map<K, V>::iterator _ret;
        _ret._mutex_stats = &_mutex_stats;
        _mutex_stats.TryLock();
        _ret = this->begin();
        return _ret;
    }

    template<class K, class V>
    BusMap_Declar_Typename busmap::map<K, V>::iterator busmap::map<K, V>::find_s(const K &key_)
    {
        busmap::map<K, V>::iterator _ret;
        _ret._mutex_stats = &_mutex_stats;
        _mutex_stats.TryLock();
        _ret = this->find(key_);
        return _ret;
    }

    template<class K, class V>
    void busmap::map<K, V>::erase_s(BusMap_Declar_Typename busmap::map<K, V>::iterator &itr_)
    {
        _mutex_stats.TryLock();
        this->erase(itr_);
        _mutex_stats.TryUnLock();
    }

    template<class K, class V>
    void busmap::map<K, V>::clear_s()
    {
        _mutex_stats.TryLock();
        this->clear();
        _mutex_stats.TryUnLock();
    }

    template<class K, class V>
    void busmap::map<K, V>::erase_s(const K &key_)
    {
        _mutex_stats.TryLock();
        this->erase(key_);
        _mutex_stats.TryUnLock();
    }

    template<class K, class V>
    void busmap::map<K, V>::insert_s(const K &key_, const V &val_)
    {
        _mutex_stats.TryLock();
        this->insert(std::map<K, V>::value_type(key_, val_));
        _mutex_stats.TryUnLock();
    }

    template<class K, class V>
    busmap::map<K, V> &busmap::map<K, V>::operator=(const map &val_)
    {
        val_._mutex_stats.TryLock();
        this->std::map<K, V>::operator=(val_);
        val_._mutex_stats.TryUnLock();
        return *this;
    }

    template<class K, class V>
    busmap::map<K, V>::~map() { }

    template<class K, class V>
    busmap::map<K, V>::map(const busmap::map<K, V> &val_)
    {
        val_._mutex_stats.TryLock();
        this->std::map<K, V>::operator=(val_);
        val_._mutex_stats.TryUnLock();
    }

    template<class K, class V>
    busmap::map<K, V>::map() { }
}

#endif  //__BUS_MAP_H__