/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : busmap.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-15
  *Desc        : ����STL��׼Map�����̰߳�ȫ�ģ��ֽ���ʵ��Ϊ�̰߳�ȫ���ͣ��Է���ʹ��
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

        //��_mutex_usr.Lock������_mutex_stats�ͷź󣬷�ֹ����
        //�����зǳɶԳ��ֵ��������������ڶ��̵߳���ʱһ�����������
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

    //��������ֵ�Ϳ�������ʱLock������ʱ��Ҫ�ͷ���
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