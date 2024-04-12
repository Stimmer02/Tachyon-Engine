#ifndef KEYBOARDMONITOR_H
#define KEYBOARDMONITOR_H

#include "EventInfo.h"
#include "WindowContext.h"
#include <GLFW/glfw3.h>
#include <cstring>
#include <cassert>

class MouseButtonMonitor{

private:

    WindowContext * window;

    double lastXPosition;
    double lastYPosition;

    int width;
    int height;

    char lastMouseStates[GLFW_MOUSE_BUTTON_LAST + 1];

public:

    MouseButtonMonitor(WindowContext * context);

    EventInfo Query(int button);

    ~MouseButtonMonitor();

};


#endif
