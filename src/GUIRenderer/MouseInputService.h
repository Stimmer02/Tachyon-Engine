#ifndef MOUSEINPUTSERVICE_H
#define MOUSEINPUTSERVICE_H

#include "IInputHandler.h"

class MouseInputService : public IInputHandler {

public:

    MouseInputService();

    void BindWindow(GLFWwindow * _window);

    void SetNormalCursor(const unsigned char* pixels, const size_t & width, const size_t & height);

    void SetActiveCursor(const unsigned char* pixels, const size_t & width, const size_t & height);

    EventInfo Query();

    ~MouseInputService();

private:

    GLFWwindow * window;

    GLFWcursor * normal;
    GLFWcursor * active;

    double lastXPosition;
    double lastYPosition;

    int width;
    int height;


};


#endif
