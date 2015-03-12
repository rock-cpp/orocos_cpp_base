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
#include <fstream>
#include <boost/tokenizer.hpp>

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

bool OrocosHelpers::parsePkgConfig(const std::string& pkgConfigFileName, const std::vector< std::string > &searchedFields, std::vector< std::string > &result)
{
    const char *pkgConfigPath = getenv("PKG_CONFIG_PATH");
    if(!pkgConfigPath)
    {
        throw std::runtime_error("Internal Error, no pkgConfig path found.");
    }
    
    std::string pkgConfigPathS = pkgConfigPath;
    
    boost::char_separator<char> sep(":");
    boost::tokenizer<boost::char_separator<char> > paths(pkgConfigPathS, sep);
    
    std::string searchedPath;
    
    for(const std::string &path: paths)
    {
        std::string candidate = path + "/" + pkgConfigFileName;
        if(boost::filesystem::exists(candidate))
        {
            searchedPath = candidate;
            break;
        }
    }

    if(searchedPath.empty())
    {
        throw std::runtime_error("Error, could not find pkg-config file " + pkgConfigFileName + " in the PKG_CONFIG_PATH");
    }
    
    std::vector<bool> found;
    result.resize(searchedFields.size());
    found.resize(searchedFields.size(), false);
    
    std::ifstream fileStream(searchedPath);
    
    while(!fileStream.eof())
    {
        std::string curLine;
        std::getline(fileStream, curLine);
        for(size_t i = 0; i < searchedFields.size(); i++)
        {
            const std::string searched(searchedFields[i] + "=");
            if(curLine.substr(0, searched.size()) == searched)
            {
                result[i] = curLine.substr(searched.size(), curLine.size());
                found[i] = true;
                
                //check if we found all search values
                bool doReturn = true;
                for(bool f: found)
                {
                    doReturn &= f;
                }
                if(doReturn)
                    return true;
            }
        }        
    }
    
    return false;
}


#include <rtt/rtt-config.h>
#include <string>

bool solveString(std::string &input, const std::string &replace, const std::string &by)
{
    size_t start_pos = input.find(replace);
    if(start_pos == std::string::npos)
        return false;
    
    input.replace(start_pos, replace.length(), by);
    return true;
}
#define xstr(s) str(s)
#define str(s) #s


bool OrocosHelpers::loadTypekitAndTransports(const std::string& componentName)
{
    std::vector<std::string> knownTransports;
    knownTransports.push_back("corba");
    knownTransports.push_back("mqueue");
    knownTransports.push_back("typelib");
    
    //first we load the typekit
    std::vector<std::string> pkgConfigFields;
    pkgConfigFields.push_back("prefix");
    pkgConfigFields.push_back("libdir");
    std::vector<std::string> pkgConfigValues;

    RTT::plugin::PluginLoader &loader(*RTT::plugin::PluginLoader::Instance());

    if(componentName == "rtt-types")
    {
        //special case, rtt does not follow the convention below
        if(!parsePkgConfig("orocos-rtt-" xstr(OROCOS_TARGET) ".pc", pkgConfigFields, pkgConfigValues))
            throw std::runtime_error("Could not load pkgConfig file for typekit for component " + componentName);
    
        if(!loader.loadTypekits(pkgConfigValues[0] + "/lib/orocos/gnulinux/"))
            throw std::runtime_error("Error, failed to load rtt basis typekits");

        if(!loader.loadPlugins(pkgConfigValues[0] + "/lib/orocos/gnulinux/"))
            throw std::runtime_error("Error, failed to load rtt basis plugins");
        
        return true;
    }
    
    if(!parsePkgConfig(componentName + std::string("-typekit-") + xstr(OROCOS_TARGET) + std::string(".pc"), pkgConfigFields, pkgConfigValues))
        throw std::runtime_error("Could not load pkgConfig file for typekit for component " + componentName);

    std::string libDir = pkgConfigValues[1];
    if(!solveString(libDir, "${prefix}", pkgConfigValues[0]))
        throw std::runtime_error("Internal Error while parsing pkgConfig file");
    
        
    if(!loader.loadLibrary(libDir + "/lib" + componentName + "-typekit-" xstr(OROCOS_TARGET) ".so"))
        throw std::runtime_error("Error, could not load typekit for component " + componentName);

    for(const std::string &transport: knownTransports)
    {
        if(!parsePkgConfig(componentName + "-typekit-" xstr(OROCOS_TARGET) ".pc", pkgConfigFields, pkgConfigValues))
            throw std::runtime_error("Could not load pkgConfig file for transport " + transport + " for component " + componentName);

        std::string libDir = pkgConfigValues[1];
        if(!solveString(libDir, "${prefix}", pkgConfigValues[0]))
            throw std::runtime_error("Internal Error while parsing pkgConfig file");
        
        RTT::plugin::PluginLoader &loader(*RTT::plugin::PluginLoader::Instance());
            
        if(!loader.loadLibrary(libDir + "/lib" + componentName + "-transport-" + transport + "-" xstr(OROCOS_TARGET) ".so"))
            throw std::runtime_error("Error, could not load transport " + transport + " for component " + componentName);

    }
    
    return true;
}
