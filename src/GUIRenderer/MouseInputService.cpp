#include "MouseInputService.h"


void MouseInputService::BindWindow(GLFWwindow * _window){
    this->window = _window;
}   

EventInfo MouseInputService::Query(){
    EventInfo info;

    //No event happened
    info.type = NONE;

    //Read current mouse position
    glfwGetCursorPos(window, &info.x, &info.y); 
    //Read current window size
    glfwGetWindowSize(window, &width, &height);

    //Apply vertical correction
    info.y=height-info.y; 
    
    // Check if the mouse has moved
    bool hasMoved = (lastXPosition != info.x || lastYPosition != info.y);

    // Check if the left mouse button is pressed
    bool isButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (hasMoved) {
        info.type = ONMOVE;
    }else{
        info.type = ONFOCUS;
    }

    if (isButtonPressed) {
        info.type = ONCLICK;
    }

    //Remember last mouse position
    lastXPosition = info.x;
    lastYPosition = info.y;

    return info;
}