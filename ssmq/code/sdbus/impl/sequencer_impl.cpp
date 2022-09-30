
#include "sequencer_impl.h"
#include "boost/thread.hpp"

namespace sdbus{

SequencerImpl::SequencerImpl()
{
    head_ = tail_ = NULL ;
    last_sequence_ = 0 ;
    rtt_ = 0 ;
    d_ = 0 ;
}

SequencerImpl::~SequencerImpl()
{
    boost::unique_lock<boost::shared_mutex> lock(guard_) ;    

    Node * node = NULL ;
    while((node = head_) != NULL)
    {
        head_ = node->Next ;

        Message * msg = node->Data ;
        if(msg != NULL)
            delete msg ;

        delete node ;        
    }

    head_ = tail_ = NULL ;
}

bool SequencerImpl::StartSequence(uint32_t seq) 
{
    last_sequence_ = seq - 1 ;
    return true ;
}

SequencerImpl::Node::Node() 
{
    ::memset(this , 0 , sizeof(Node)) ;
}

SequencerImpl::Node::~Node() 
{
    if(Data != NULL)
    {
        delete Data ;
        Data = NULL ;
    }
}


SequencerImpl::Iterator::Iterator()
{
    node_ = NULL ;
}

SequencerImpl::Iterator::~Iterator()
{
    if(node_ != NULL)
    {
        long v = node_->decrement() ;
        if(v == 0)
        {
            delete node_ ;
            node_ = NULL ;
        }
    }
}

SequencerImpl::Iterator& SequencerImpl::Iterator::operator=(SequencerImpl::Node * node)
{
    if(node != NULL)
    {
        node->increment() ;
        node_ = node ;
    }

    return (*this) ;
}

Message * SequencerImpl::Iterator::Get() const
{
    if(node_ != NULL)
        return node_->Data ;
    return NULL ;
}

void SequencerImpl::Register(Message * msg , uint32_t seq)
{
    Node * node = new Node() ;
    node->Data = msg ;
    node->Sequence = seq ;
    node->Time = SequencerImpl::UNow() ;

    boost::unique_lock<boost::shared_mutex> lock(guard_) ;
    if((int)(seq - last_sequence_) <= 0)
    {
        delete node ;
        return ;
    }

    node->increment() ;
    if(tail_ == NULL)
    {
        head_ = tail_ = node ;
        return ;
    }

    //通常状态下，seq总是比当前最后一个节点要大，因此
    //插入消息时，要从后往前遍历。
    Node * last = tail_ ;
    while(last != NULL)
    {
        int dist = (int)(last->Sequence - seq) ;
        if(dist == 0)
        {
            //重复，要删除该消息
            delete node ;
            return ;
        }
        else if(dist < 0)
        {
            //找到第一个小于seq的节点，那么将新节点插入该节点之后
            node->Next = last->Next ;
            if(last->Next != NULL)
                last->Next->Prev = node ;

            node->Prev = last ;
            last->Next = node ;

            if(last == tail_)
                tail_ = node ;
            return ;
        }

        last = last->Prev ;
    }

    if(last == NULL)
    {
        //找到队头了
        node->Next  = head_ ;
        head_->Prev = node ;
        
        head_ = node ;
    }
}

bool SequencerImpl::Confirm(uint32_t from , uint32_t to)
{
    int64_t now = UNow() ;

    boost::unique_lock<boost::shared_mutex> lock(guard_) ;

    /*
        打上确认标记，如果是连续有序的，那么就删除节点，同时重置最后一个序列号
    */
    Node * cur = head_ ;
    while(cur != NULL)
    {
        uint32_t seq = cur->Sequence ;
        if((int)(seq - from) >= 0 && (int)(to - seq) >= 0)
        {
            if((cur->Flag & 1) == 0)
            {
                //首次确认，不是重传
                AddM(cur->Time , now) ;
                //在有效范围内
                cur->Flag |= 1 ;
            }
        }

        if((cur->Flag & 1) == 1)
        {
            if(seq == last_sequence_ + 1)
            {
                //有序区间都被确认，那么就删除。
                head_ = cur->Next ;
                if(head_ != NULL)
                {
                    head_->Prev = NULL ;                    
                }
                else
                {
                    tail_ = NULL ;
                }

                ++last_sequence_ ;

                long v = cur->decrement() ;
                if(v == 0)
                    delete cur ;
                cur = head_ ;
            }        
            else
            {
                cur = cur->Next ;                 
            }
        }
        else
        {
            break ;
        }
    }//end while ...

    return true ;    
}

int SequencerImpl::Sequenced(Message ** msgs , size_t count)
{
    int mcount = 0 ;
    boost::unique_lock<boost::shared_mutex> lock(guard_) ;

    Node * cur = head_ ;
    while(cur != NULL && mcount < (int)count)
    {
        uint32_t seq = cur->Sequence ;
        if(seq == last_sequence_ + 1)
        {
            //连续区间
            head_ = cur->Next ;
            if(head_ != NULL)
                head_->Prev = NULL ;
            else
                tail_ = NULL ;
            ++last_sequence_ ;
            msgs[mcount++] = cur->Data ;
            cur->Data = NULL ;

            long v= cur->decrement() ;
            if(v == 0)
                delete cur ;

            cur = head_ ;
        }
        else
        {
            break ;
        }    
    }

    return mcount ;
}

int SequencerImpl::CheckUnconfirmed(Iterator * iters , size_t count , int usec) const
{
    int64_t now = UNow() ;
    boost::shared_lock<boost::shared_mutex> lock(guard_) ;

    int mcount = 0 ;
    Node * cur = head_ ;
    while(cur != NULL && mcount < (int)count)
    {
        if((cur->Flag & 1) == 0 && (now - cur->Time) >= usec)
        {
            cur->RetransmitTimes += 1 ;
            iters[mcount++] = cur ;
        }

        cur = cur->Next ;    
    }

    return mcount ;
}

bool SequencerImpl::CheckHole(uint32_t& from , uint32_t& to , int usec) const
{
    int64_t now = UNow() ;
    bool result = false ;

    boost::shared_lock<boost::shared_mutex> lock(guard_) ;

    Node * node = head_ ;
    uint32_t last = last_sequence_ ;
    while(node != NULL)
    {
        if((now - node->Time) < usec)
        {
            break ;            
        }

        uint32_t seq = node->Sequence ;
        if(seq == last + 1)
            last = seq ;
        else
        {
            from = last + 1 ;
            to = seq - 1 ;
            result = true ;
        }

        node = node->Next ;
    }

    return result ;
}


int64_t SequencerImpl::UNow() 
{
    time_t sec = 0 ;
    int usec = 0 ;

    ss::unow(sec , usec) ;

    int64_t result = (int)sec ;
    result *= 1000000 ;
    result += usec ;

    return result ;
}

const double SequencerImpl::ALPHA = 0.875 ;

void SequencerImpl::AddM(int64_t st , int64_t ct) 
{
    int64_t delta = ct - st ;
    if(delta <= 0)
        return ;

    if(rtt_ == 0)
    {
        rtt_ = delta ;
        return ;
    }
    
    double r = ALPHA * (double)rtt_ + (1 - ALPHA) * (double)delta ;
    rtt_ = (int64_t)r ;

    int64_t ad = rtt_ - delta ;
    if(ad < 0)
        ad = -ad ;

    double d = ALPHA * (double)d_ + (1 - ALPHA) * (double)ad ;
    d_ = (int64_t)d ;
}

int64_t SequencerImpl::CalcRTO() const 
{
    return rtt_ + 4 * d_ ;
}

}
