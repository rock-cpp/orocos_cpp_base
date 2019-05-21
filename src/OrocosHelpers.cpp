#include "OrocosHelpers.hpp"

#include <rtt/TaskContext.hpp>
#include <rtt/transports/corba/TaskContextServer.hpp>
#include <rtt/transports/corba/CorbaDispatcher.hpp>
#include <rtt/plugin/PluginLoader.hpp>
static RTT::TaskContext *clientTask = NULL;
#include <sys/types.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>

void OrocosHelpers::initClientTask(const std::string& name)
{
    if(clientTask)
    {
        throw std::runtime_error("OrocosHelpers::initClientTask : Error, initClientTask was called, and the client was already initialized");
    }
    
    const char *ldLibPath = getenv("RTT_COMPONENT_PATH");
    if(ldLibPath)
    {
        RTT::plugin::PluginLoader::Instance()->setPluginPath(ldLibPath);
    }   

    clientTask = new RTT::TaskContext(name);
    
    RTT::corba::TaskContextServer::Create( clientTask );

#if RTT_VERSION_GTE(2,8,99)
    clientTask->addConstant<int>("CorbaDispatcherScheduler", ORO_SCHED_OTHER);
    clientTask->addConstant<int>("CorbaDispatcherPriority", RTT::os::LowestPriority);
#else
    RTT::corba::CorbaDispatcher::Instance( clientTask->ports(), ORO_SCHED_OTHER, RTT::os::LowestPriority );
#endif
}


RTT::TaskContext* OrocosHelpers::getClientTask()
{
    if(!clientTask)
    {
        pid_t pid = getpid();
        initClientTask("OrocosCPP_" + boost::lexical_cast<std::string>(pid));
    }
    return clientTask;
}

