#ifndef MOUSEBUTTONMONITOR_H
#define MOUSEBUTTONMONITOR_H

#include "IShareableContext.h"
#include "EventInfo.h"

#include <cstring>

class MouseButtonMonitor : public IShareableContext {

private:

    GLFWwindow * window;

    GLFWcursor * normal;
    GLFWcursor * active;

    double lastXPosition;
    double lastYPosition;

    int width;
    int height;

    int lastStates[GLFW_MOUSE_BUTTON_LAST];

public:

    MouseButtonMonitor();

    void AcceptGLFWContext( GLFWwindow * window );

    void SetNormalCursor(const unsigned char* pixels, const size_t & width, const size_t & height);

    void SetActiveCursor(const unsigned char* pixels, const size_t & width, const size_t & height);

    void Update();

    EventInfo Query(int button);

    ~MouseButtonMonitor();

};


#endif
