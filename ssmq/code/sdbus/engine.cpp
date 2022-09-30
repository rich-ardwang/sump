
#include "sdbus/engine.h"
#include <boost/thread/mutex.hpp>
#include <map>

namespace sdbus{

Engine::Engine()
{
    Creator = NULL ;
    Destroyer = NULL ;

    Initializer = NULL ;
    Finalizer = NULL ;

    AddHandler = NULL ;
    DelHandler = NULL ;

    AsyncSend = NULL ;
    SyncSend = NULL ;

    Publish = NULL ;
    SetOptions = NULL ;
    GetOptions = NULL ;

    PrivData = NULL ;
}

Engine::~Engine()
{
    //
}

void Engine::Assign(const Engine& engine) 
{
    Creator = engine.Creator ;
    Destroyer = engine.Destroyer ;

    Initializer = engine.Initializer ;
    Finalizer = engine.Finalizer ;

    AddHandler = engine.AddHandler ;
    DelHandler = engine.DelHandler ;

    AsyncSend = engine.AsyncSend ;
    SyncSend = engine.SyncSend ;

    Publish = engine.Publish ;
    SetOptions = engine.SetOptions ;
    GetOptions = engine.GetOptions ;

    PrivData = NULL ;
}

class EngineManager{
public:
    EngineManager() {}
    ~EngineManager() {}

    bool AddEngine(const sdbus::string& name , const Engine& engine)
    {
        boost::mutex::scoped_lock lock(guard_) ;
        std::map<sdbus::string , Engine>::iterator iter = engines_.find(name) ;
        if(iter != engines_.end())
            return false ;

        return engines_.insert(std::pair<sdbus::string , Engine>(name , engine)).second ;
    }

    bool DelEngine(const sdbus::string& name)
    {
        boost::mutex::scoped_lock lock(guard_) ;
        std::map<sdbus::string , Engine>::iterator iter = engines_.find(name) ;
        if(iter == engines_.end())
            return false ;    

        engines_.erase(iter) ;
        return true ;
    }

    Engine * Find(const sdbus::string& name)
    {
        boost::mutex::scoped_lock lock(guard_) ;
        std::map<sdbus::string , Engine>::iterator iter = engines_.find(name) ;
        if(iter == engines_.end())
            return NULL ;    
        return &iter->second ;
    }

    static EngineManager& Singleton()
    {
        static EngineManager __EngineManager_Singleton__ ;
        return __EngineManager_Singleton__ ;    
    }

private:
    boost::mutex guard_ ;
    std::map<sdbus::string , Engine> engines_ ;
} ;

bool RegisterEngine(const sdbus::string& name , const Engine& engine)
{
    return EngineManager::Singleton().AddEngine(name , engine) ;
}

bool UnregisterEngine(const sdbus::string& name)
{
    return EngineManager::Singleton().DelEngine(name) ;
}

Engine * CreateEngine(const sdbus::string& name)
{
    Engine * engine = EngineManager::Singleton().Find(name) ;
    if(engine == NULL)
        return NULL ;

    return engine->Creator() ;
}

void DestroyEngine(Engine * engine)
{
    engine->Destroyer(engine) ;
}

}

