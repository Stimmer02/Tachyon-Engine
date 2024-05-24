#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H

#include <GLFW/glfw3.h>

#include "EventInfo.h"

class IInputHandler {

public:

    /// @brief Method returns structure which describe event type and last known position of that event
    /// @return
    virtual EventInfo Query() = 0;

};


#endif
