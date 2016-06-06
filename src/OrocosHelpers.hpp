#ifndef OROCOSHELPERS_H
#define OROCOSHELPERS_H
#include <string>
#include <vector>

namespace RTT
{
    class TaskContext;
}



class OrocosHelpers
{
public:

    /***
     * Initializes the client task of this process 
     * with the given name. MUST be called prior 
     * to getClientTask().
     * */
    static void initClientTask(const std::string &name);
    

    /**
     * Returns the TaskContext that is used for the client side.
     * */
    static RTT::TaskContext *getClientTask();    
};

#endif // OROCOSHELPERS_H
