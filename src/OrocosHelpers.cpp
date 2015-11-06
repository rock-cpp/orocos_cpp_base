#include "OrocosHelpers.hpp"

#include <rtt/TaskContext.hpp>
#include <rtt/transports/corba/TaskContextServer.hpp>
#include <rtt/transports/corba/CorbaDispatcher.hpp>
#include <rtt/plugin/PluginLoader.hpp>
static RTT::TaskContext *clientTask = NULL;
#include <sys/types.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>

RTT::TaskContext* OrocosHelpers::getClientTask()
{
    if(!clientTask)
    {
        const char *ldLibPath = getenv("RTT_COMPONENT_PATH");
        if(ldLibPath)
        {
            RTT::plugin::PluginLoader::Instance()->setPluginPath(ldLibPath);
        }   

        pid_t pid = getpid();
        clientTask = new RTT::TaskContext("OrocosCPP_" + boost::lexical_cast<std::string>(pid) );
        
        RTT::corba::TaskContextServer::Create( clientTask );
    
        RTT::corba::CorbaDispatcher::Instance( clientTask->ports(), ORO_SCHED_OTHER, RTT::os::LowestPriority );
    }
    return clientTask;
}


