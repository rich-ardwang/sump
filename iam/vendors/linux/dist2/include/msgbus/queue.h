
#ifndef __MSGBUS_QUEUE_H
#define __MSGBUS_QUEUE_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "msgbus/message.h"

namespace msgbus {

class queue_impl ;
class MSGBUSAPI queue {
public:
    queue() ;
    virtual ~queue() ;

    bool enqueue(void * data) ;
    void * dequeue() ;
    int dequeue(void * datas[] , int count) ;

    void clear() ;

    size_t size() const  ;
    int backlog() const ;

    typedef void (*RECLAIM_CALLBACK) (void * data) ;

    void reclaim(RECLAIM_CALLBACK rec) ;

    size_t enq_counter() const ;
    size_t deq_counter() const ;

    inline queue_impl * impl() {return impl_ ;}
private:    
    queue_impl * impl_ ;
} ;

class queue_manager_impl ;
class MSGBUSAPI queue_manager{
public :
    queue_manager() ;
    ~queue_manager() ;

    queue * find_queue(const std::string& name) ;
    queue * add_queue(const std::string& name , queue * q) ;
    void del_queue(const std::string& name) ;
    void del_queue_clean(const std::string& name) ;
	
    void clear() ;

    int wait(queue * queues[] , int count , int timeout = 0 ) ;//second

    bool empty() const ;

    int notify_counter() const ;
    int link_counter() const ;
    int unlink_counter() const ;
    int check_link_counter() const ;

    void list_names(std::vector<std::string>& names) const;
    void list_queues(std::vector<queue *>& queues) const ;
private:
    queue_manager_impl * impl_ ;
} ;


}

#endif  /** __MSGBUS_QUEUE_H */

