#include "OrocosHelpers.hpp"

#include <rtt/TaskContext.hpp>
#include <rtt/transports/corba/TaskContextServer.hpp>
#include <rtt/transports/corba/CorbaDispatcher.hpp>
#include <rtt/plugin/PluginLoader.hpp>
static RTT::TaskContext *clientTask = NULL;

RTT::TaskContext* OrocosHelpers::getClientTask()
{
    if(!clientTask)
    {
        const char *ldLibPath = getenv("RTT_COMPONENT_PATH");
        if(ldLibPath)
        {
            RTT::plugin::PluginLoader::Instance()->setPluginPath(ldLibPath);
        }   

        clientTask = new RTT::TaskContext("OrocosCPP");
        
        RTT::corba::TaskContextServer::Create( clientTask );
    
        RTT::corba::CorbaDispatcher::Instance( clientTask->ports(), ORO_SCHED_OTHER, RTT::os::LowestPriority );
    }
    return clientTask;
}


