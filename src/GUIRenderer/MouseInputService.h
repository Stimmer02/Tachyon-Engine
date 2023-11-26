#ifndef MOUSEINPUTSERVICE_H
#define MOUSEINPUTSERVICE_H

#include "IInputHandlingService.h"

class MouseInputService : public IInputHandlingService {

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