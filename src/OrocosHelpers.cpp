#include "OrocosHelpers.hpp"

#include <rtt/TaskContext.hpp>

static RTT::TaskContext *clientTask = NULL;

RTT::TaskContext* OrocosHelpers::getClientTask()
{
    if(!clientTask)
    {
        clientTask = new RTT::TaskContext("OrocosCPP");
    }
    return clientTask;
}


