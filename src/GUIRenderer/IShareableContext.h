#ifndef ISHAREABLECONTEXT_H
#define ISHAREABLECONTEXT_H

#include <GLFW/glfw3.h>

class IShareableContext{
public:

    virtual void AcceptGLFWContext( GLFWwindow * window ) = 0;

};

#endif
