
#ifndef __SDBUS_SEQUENCER_IMPL_H
#define __SDBUS_SEQUENCER_IMPL_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include "sdbus/message.h"

#include "SSRTL/SSDateTime.h"
#include "SSRTL/SSAtomic.h"
#include <time.h>
#include "boost/thread/shared_mutex.hpp"

namespace sdbus{

/**
    2014-08-06
    为了防止序列号空洞太大，不能使用数组。
    由于丢包只会发生在小范围内，因此，用链表就可以解决，
    而不需要排序算法。

    由于序列号存在回绕的问题。因此在判断是否有序的算法不能是
    seq1 > seq2 ，而是 (int)(seq1 - seq2) > 0 ，请注意。
*/
class SequencerImpl{
public:
    SequencerImpl() ;
    ~SequencerImpl() ;

    bool StartSequence(uint32_t seq) ;

    /*
        向序列器报告自己，进入排序机制。
        如果小于当前序列号，或者和现有的重复，那么将被内部释放。
    */
    void Register(Message * msg , uint32_t seq) ;

    //用来标记对端发送来的确认消息，表示对端已经成功接收到本端发送的消息。
    bool Confirm(uint32_t from , uint32_t to) ;

    /*
        返回已经排序过的内容。主要用来标记对端发送来消息，经过排序重组之后，
        返回给应用层。
    */
    int Sequenced(Message ** msgs , size_t count) ;

    class Node : public ss::atomic_count {
    public:
        Node() ;
        ~Node() ;

        Node *    Prev ;
        Node *    Next ;
        Message * Data ;
        int64_t   Time ;                //精确到微妙
        uint32_t  Sequence ;    
        int       Flag ;                //0表示未被确认的
        int       RetransmitTimes ;     //重传次数
    };

    class Iterator{
    public:
        Iterator() ;
        ~Iterator() ;    
        Iterator& operator=(Node * node) ;

        Message * Get() const ;
    private:
        Node * node_ ;
    } ;

    /*
        检查那些未被确认的消息，用来做重传的。
    */
    int CheckUnconfirmed(Iterator * iters , size_t count , int usec = 0) const;

    /*
        检查那些未连续的消息序列空洞。
    */
    bool CheckHole(uint32_t& from , uint32_t& to , int usec = 0) const;

    int64_t CalcRTO() const ;

    static const double ALPHA  ;
    static int64_t UNow() ;
private:

    Node * head_ ;
    Node * tail_ ;

    uint32_t last_sequence_ ;       //最后一个被可靠有序传输完成的序列号。

    mutable boost::shared_mutex guard_ ;

    /*
        超时重传的几个变量
        RTT : round trip time  
        RTO : retransmission time out
        D : 平滑的平均偏差
        ALPHA : 平滑因子，通常为7/8
        M ： 一个数据段从发送到收到确认的时间差。
        RTT = ALPHA * RTT + (1 - ALPHA) M 
        D = ALPHA * D + (1 - ALPHA) * | RTT - M |
        RTO = RTT + 4D

        karn 算法：
        1、对于发生重传的数据段，在收到确认后，不更新RTT
        2、对于重传的时候，RTO是倍增的，直到达到最大值的限制。如果重传超过一定的次数，TCP连接会断开
        3、在重传并收到确认后，如果下一次的数据段没有发生重传，则又恢复jacobson算法。
    */
    int64_t rtt_ ;      
    int64_t d_ ;

    void AddM(int64_t st , int64_t ct) ;
} ;

}

#endif  /** __SDBUS_SEQUENCER_IMPL_H */
