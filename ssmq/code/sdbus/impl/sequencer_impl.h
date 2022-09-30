
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
    Ϊ�˷�ֹ���кſն�̫�󣬲���ʹ�����顣
    ���ڶ���ֻ�ᷢ����С��Χ�ڣ���ˣ�������Ϳ��Խ����
    ������Ҫ�����㷨��

    �������кŴ��ڻ��Ƶ����⡣������ж��Ƿ�������㷨������
    seq1 > seq2 ������ (int)(seq1 - seq2) > 0 ����ע�⡣
*/
class SequencerImpl{
public:
    SequencerImpl() ;
    ~SequencerImpl() ;

    bool StartSequence(uint32_t seq) ;

    /*
        �������������Լ�������������ơ�
        ���С�ڵ�ǰ���кţ����ߺ����е��ظ�����ô�����ڲ��ͷš�
    */
    void Register(Message * msg , uint32_t seq) ;

    //������ǶԶ˷�������ȷ����Ϣ����ʾ�Զ��Ѿ��ɹ����յ����˷��͵���Ϣ��
    bool Confirm(uint32_t from , uint32_t to) ;

    /*
        �����Ѿ�����������ݡ���Ҫ������ǶԶ˷�������Ϣ��������������֮��
        ���ظ�Ӧ�ò㡣
    */
    int Sequenced(Message ** msgs , size_t count) ;

    class Node : public ss::atomic_count {
    public:
        Node() ;
        ~Node() ;

        Node *    Prev ;
        Node *    Next ;
        Message * Data ;
        int64_t   Time ;                //��ȷ��΢��
        uint32_t  Sequence ;    
        int       Flag ;                //0��ʾδ��ȷ�ϵ�
        int       RetransmitTimes ;     //�ش�����
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
        �����Щδ��ȷ�ϵ���Ϣ���������ش��ġ�
    */
    int CheckUnconfirmed(Iterator * iters , size_t count , int usec = 0) const;

    /*
        �����Щδ��������Ϣ���пն���
    */
    bool CheckHole(uint32_t& from , uint32_t& to , int usec = 0) const;

    int64_t CalcRTO() const ;

    static const double ALPHA  ;
    static int64_t UNow() ;
private:

    Node * head_ ;
    Node * tail_ ;

    uint32_t last_sequence_ ;       //���һ�����ɿ���������ɵ����кš�

    mutable boost::shared_mutex guard_ ;

    /*
        ��ʱ�ش��ļ�������
        RTT : round trip time  
        RTO : retransmission time out
        D : ƽ����ƽ��ƫ��
        ALPHA : ƽ�����ӣ�ͨ��Ϊ7/8
        M �� һ�����ݶδӷ��͵��յ�ȷ�ϵ�ʱ��
        RTT = ALPHA * RTT + (1 - ALPHA) M 
        D = ALPHA * D + (1 - ALPHA) * | RTT - M |
        RTO = RTT + 4D

        karn �㷨��
        1�����ڷ����ش������ݶΣ����յ�ȷ�Ϻ󣬲�����RTT
        2�������ش���ʱ��RTO�Ǳ����ģ�ֱ���ﵽ���ֵ�����ơ�����ش�����һ���Ĵ�����TCP���ӻ�Ͽ�
        3�����ش����յ�ȷ�Ϻ������һ�ε����ݶ�û�з����ش������ָֻ�jacobson�㷨��
    */
    int64_t rtt_ ;      
    int64_t d_ ;

    void AddM(int64_t st , int64_t ct) ;
} ;

}

#endif  /** __SDBUS_SEQUENCER_IMPL_H */
