/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : BlockingQueue.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-16
  *Desc        : 实现阻塞式并发队列，由于并发量不大，暂时不需要实现队列Buf上限
  *History     : Add by Lei.Wang 2019-05-16
***************************************************************************************************/
#pragma once

#include <deque>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

using std::deque;

template<typename T>
class BlockingQueue : boost::noncopyable
{
public:
    BlockingQueue() 
        : mutex_()
        , queue_not_empty_()
        , queue_()
    {}

    void put(const T& x)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        queue_.push_back(x);
        queue_not_empty_.notify_one();
    }

    T take()
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        while (queue_.empty())
        {
            queue_not_empty_.wait(mutex_);
        }
        assert(!queue_.empty());
        T front(queue_.front());
        queue_.pop_front();

        return front;
    }

    size_t size() const
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    boost::mutex mutex_;
    boost::condition queue_not_empty_;
    deque<T> queue_;
};

