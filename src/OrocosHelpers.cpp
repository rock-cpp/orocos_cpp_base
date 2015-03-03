#include "OrocosHelpers.hpp"

#include <rtt/typekit/RealTimeTypekit.hpp>
#include <rtt/transports/corba/TransportPlugin.hpp>
#include <rtt/transports/mqueue/TransportPlugin.hpp>

#include <rtt/types/TypekitPlugin.hpp>
    
#include <boost/filesystem.hpp>
#include <rtt/plugin/PluginLoader.hpp>
#include <rtt/InputPort.hpp>

#include <rtt/TaskContext.hpp>

#include <base/Time.hpp>

static RTT::TaskContext *clientTask = NULL;

RTT::TaskContext* getClientTask()
{
    if(!clientTask)
    {
        clientTask = new RTT::TaskContext("OrocosCPP");
    }
    return clientTask;
}


void loadAllPluginsInDir(const std::string &path)
{
    base::Time start = base::Time::now();
    boost::filesystem::path pluginDir(path);

    int cnt = 0;
    boost::shared_ptr<RTT::plugin::PluginLoader> loader = RTT::plugin::PluginLoader::Instance();
    
    boost::filesystem::directory_iterator end_it; // default construction yields past-the-end
    for (boost::filesystem::directory_iterator it( pluginDir );
        it != end_it; it++ )
    {
        if(boost::filesystem::is_regular_file(*it))
        {
//             std::cout << "Found library " << *it << std::endl;
            loader->loadLibrary(it->path().string());
            cnt++;
        }
    }
    base::Time end = base::Time::now();

    std::cout << "Loaded " << cnt << " typekits in " << (end - start).toSeconds() << " Seconds " << std::endl; 
}
