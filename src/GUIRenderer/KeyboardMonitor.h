#ifndef MOUSEBUTTONMONITOR_H
#define MOUSEBUTTONMONITOR_H

#include "EventInfo.h"
#include "WindowContext.h"
#include <GLFW/glfw3.h>
#include <cassert>

#include <cstring>

class KeyboardMonitor {

private:

    WindowContext * window;

    char lastKeyboardStates[GLFW_KEY_LAST];

public:

    KeyboardMonitor();

    KeyboardMonitor(WindowContext * window);

    EventInfo GetButtonState(int button);

    ~KeyboardMonitor();

};


#endif
