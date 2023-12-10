#ifndef IINPUTHANDLER_H
#define IINPUTHANDLER_H

#include <GLFW/glfw3.h>

#include "EventInfo.h"

class IInputHandler {

public:

    /// @brief Procedure binds specified window as input reference object
    /// @param _window 
    virtual void BindWindow(GLFWwindow * _window) = 0;

    /// @brief Method returns structure which describe event type and last known position of that event
    /// @return 
    virtual EventInfo Query() = 0;

};


#endif