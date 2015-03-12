#ifndef OROCOSHELPERS_H
#define OROCOSHELPERS_H
#include <string>
#include <vector>

namespace RTT
{
    class TaskContext;
}

RTT::TaskContext *getClientTask();

void loadAllPluginsInDir(const std::string &path);




class OrocosHelpers
{
public:
    static bool loadTypekitAndTransports(const std::string &componentName);

    /**
     * Helper function that parses a pkg-Config file. 
     * Will open the file of the given name, and search 
     * for the fields given in searchedFields. The content
     * of the field will be returned in the result vector
     * in the same order as searchedFields. 
     * */
    static bool parsePkgConfig(const std::string &pkgConfigFileName, const std::vector<std::string> &searchedFields, std::vector<std::string> &result);
};

#endif // OROCOSHELPERS_H
