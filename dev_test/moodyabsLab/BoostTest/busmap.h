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
        1.CThreadLockStats�Ǳ����˵�ǰ�����̵߳���_mutex_usr.Lock()��_mutex_usr.UnLock()
        �Ĳ�ֵ,��¼ͬһ�߳���_mutex_usr��ʹ�������
        2.���п��ú���ΪTryLock(),TryUnLock()�����ͬһ�߳���_mutex_usr�Ѿ�Lock����TryLock()
        ��������ֻ�ǽ���ʶ���������ͬһ�߳���_mutex_usr��Lock����Ϊ1ͬʱ��Ҫ�ͷ�����ʱ��
        ����TryUnLock���ͷţ���Ϊ1�򽫱�ʶ�Լ���
    */
    /************************************************************************************/
    template<class K, class V>
    class CThreadLockStats
    {
    private:
        CThreadLockStats();
        ~CThreadLockStats();

        //��ֹ����
        void TryLock();
        //��TryLock��Ӧ���ͷ���
        void TryUnLock();
        //��ȡ�߳�ID��������
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

    //busmapIterator:�������̳б�׼������,�����������һЩ����������
    template<class K, class V>
    class busmapIterator : public std::map<K, V>::iterator
    {
    public:
        busmapIterator();

        //���ؿ������죬���㿽��ʱ��Lock,�ͷ�ʱ��UnLock��
        busmapIterator(const busmapIterator &val_);
        ~busmapIterator();

        //��ֵ��������ظ��ำֵ����������̳У���Ҫ����
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
        ����iteratorĬ�Ϲ��캯��û��TryLock�������ڷ��ظ�������ʱ����ʱ��ҪTryLockһ�Σ�
        ��ʱ��������ʱTryUnLockһ�Σ������ص�ֵ���п���������߿�����ֵ��TryLockһ�Σ�ʹ��
        �������TryUnLockһ�Ρ��ﵽ�ɶ������ͷ�����Ŀ�ġ�
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