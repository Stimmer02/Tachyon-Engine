#include "MouseButtonMonitor.h"

MouseButtonMonitor::MouseButtonMonitor(WindowContext * context){
    this->window = context;
    memset(lastMouseStates, GLFW_RELEASE, sizeof(char) * GLFW_MOUSE_BUTTON_LAST);

}

MouseButtonMonitor::~MouseButtonMonitor(){


}

EventInfo MouseButtonMonitor::Query(int button){

    EventInfo info = {};

    double mouseX, mouseY;
    
    // Read current mouse position and window size
    window->GetMousePos(mouseX, mouseY);
    window->GetWindowSize(width, height);

    // Apply vertical correction
    mouseY = height-mouseY;

    if( button >= GLFW_MOUSE_BUTTON_LAST)
        return info;

    // Check if mouse position is in window boundary
    bool inWindow = (mouseX > 0.0f && mouseX <= width && mouseY > 0.0f && mouseY <= height);

    // Check if the mouse has moved
    bool positionChanged = (mouseX != lastXPosition || mouseY != lastYPosition);

    int currentState = window->GetMouseButton(button);

    // Check if the mouse button is triggered
    bool isButtonHeld = (currentState == GLFW_PRESS) && (lastMouseStates[button] == GLFW_PRESS);

    // Check if the mouse button is triggered
    bool isButtonTriggered = (currentState == GLFW_PRESS) && (lastMouseStates[button] == GLFW_RELEASE);

    // Check if the mouse button is released
    bool isButtonReleased = (currentState == GLFW_RELEASE) && (lastMouseStates[button] == GLFW_PRESS);

    if(positionChanged){
        info.type = EventType::ONMOVE;
    }else if(isButtonTriggered){
        info.type = EventType::ONTRIGGER;
    }else if(isButtonReleased){
        info.type = EventType::ONRELEASE;
    } else if (isButtonHeld){
        info.type = EventType::ONHOLD;
    }else if(inWindow){
        info.type = EventType::ONHOVER;
    }else{
        info.type = EventType::NONE;
    }

    info.x = mouseX;
    info.y = mouseY;

    // Remember last mouse position
    lastXPosition = info.x;
    lastYPosition = info.y;

    lastMouseStates[button] = currentState;

    return info;
}
