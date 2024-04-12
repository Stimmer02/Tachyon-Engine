#ifndef KEYBOARDMONITOR_H
#define KEYBOARDMONITOR_H

#include "EventInfo.h"
#include "Input.h"

#include <cstring>
#include <cassert>

class MouseButtonMonitor{

private:

    GLFWwindow * window;

    GLFWcursor * normal;
    GLFWcursor * active;

    double lastXPosition;
    double lastYPosition;

    int width;
    int height;

    char lastMouseStates[GLFW_MOUSE_BUTTON_LAST];

public:

    MouseButtonMonitor();

    void SetNormalCursor(const unsigned char* pixels, const size_t & width, const size_t & height);

    void SetActiveCursor(const unsigned char* pixels, const size_t & width, const size_t & height);

    EventInfo Query(int button);

    ~MouseButtonMonitor();

};


#endif
