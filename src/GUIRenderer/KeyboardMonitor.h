#ifndef MOUSEBUTTONMONITOR_H
#define MOUSEBUTTONMONITOR_H

#include "EventInfo.h"
#include <GLFW/glfw3.h>
#include <cassert>

#include <cstring>

class KeyboardMonitor {

private:

    GLFWwindow * window;

    char lastKeyboardStates[GLFW_KEY_LAST];

public:

    KeyboardMonitor();


    EventInfo Query(int button);

};


#endif
