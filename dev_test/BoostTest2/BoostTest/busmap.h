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
        iterator find_s(const K &key_);
        iterator begin_s();
        iterator end_s();
        unsigned int size_s();

    private:
        CThreadLockStats<K, V> _mutex_stats;
    };
}

#define BUS_MAP_TEMPLATE
#include "busmap.cpp"
#endif  //__BUS_MAP_H__