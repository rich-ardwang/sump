
#ifndef __QPID_ENGINE_NOTIFIER_H
#define __QPID_ENGINE_NOTIFIER_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition.hpp>

#include <string>
#include <map>

#include "SSRTL/SSSafeObject.h"
#include "sdbus/string.h"
#include "sdbus/message.h"

namespace engine{
	namespace zmq{

		class Notifier : public ss::SafeObject{
		public:
			Notifier() ;
			~Notifier() ;

			sdbus::Message * Wait(int timeout) ;
			void Notify(sdbus::Message * message) ;

		private:
			sdbus::Message * message_ ;
			boost::mutex guard_ ;
			boost::condition notifier_ ;
		} ;

		class NotifierManager{
		public:
			NotifierManager() ;
			~NotifierManager() ;

			typedef ss::SafeObjectRef<Notifier> NotifierType ;

			bool BeginNotifier(const sdbus::string& id) ;
			sdbus::Message * Wait(const sdbus::string& id , int timeout) ;
			bool Notify(const sdbus::string& id , sdbus::Message * msg) ;
			void RemoveNotifier(const sdbus::string& id) ;

			bool CheckMessageID(const sdbus::string& id) const ;
		private:
			std::map<sdbus::string , NotifierType> notifiers_ ;
			mutable boost::shared_mutex guard_ ;
		} ;

	}
}

#endif  /** __QPID_ENGINE_NOTIFIER_H */
