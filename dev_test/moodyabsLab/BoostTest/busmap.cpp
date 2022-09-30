#ifdef BUS_MAP_TEMPLATE

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

#endif