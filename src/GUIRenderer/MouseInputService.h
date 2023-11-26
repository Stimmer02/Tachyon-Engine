#ifndef MOUSEINPUTSERVICE_H
#define MOUSEINPUTSERVICE_H

#include "IInputHandler.h"
#include "cmath"

class MouseInputService : public IInputHandler {

    GLFWwindow * window;

    double lastXPosition;
    double lastYPosition;

    int width;
    int height;

public:

    void BindWindow(GLFWwindow * _window);

    EventInfo Query();

};


#endif