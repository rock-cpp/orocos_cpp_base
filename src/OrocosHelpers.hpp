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

    /**
     * Returns the TaskContext that is used for the client side.
     * */
    static RTT::TaskContext *getClientTask();    
};

#endif // OROCOSHELPERS_H
