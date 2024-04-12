#include "MouseButtonMonitor.h"

MouseButtonMonitor::MouseButtonMonitor(){
    this->window = glfwGetCurrentContext();
    this->normal = nullptr;
    this->active = nullptr;

    memset(lastMouseStates, GLFW_RELEASE, sizeof(char) * GLFW_MOUSE_BUTTON_LAST);

}

void MouseButtonMonitor::SetNormalCursor(const unsigned char* pixels, const size_t & width, const size_t & height){

    // Return if there is no pixels
    if(!pixels)
        return;

    // Create temporary structure which holds image info
    GLFWimage temporary;

    temporary.width = width;
    temporary.height = height;
    temporary.pixels = (unsigned char*)pixels;

    // Create new normal cursor;
    normal = glfwCreateCursor(&temporary, 0, 0);

    // Enable custom cursor within current window scope
    if(window)
        glfwSetCursor(window, normal);

}

void MouseButtonMonitor::SetActiveCursor(const unsigned char* pixels, const size_t & width, const size_t & height){

    // Return if there is no pixels
    if(!pixels)
        return;

    // Create temporary structure which holds image info
    GLFWimage temporary;

    temporary.width = width;
    temporary.height = height;
    temporary.pixels = (unsigned char*)pixels;

    // Create new normal cursor;
    active = glfwCreateCursor(&temporary, 0, 0);

}

MouseButtonMonitor::~MouseButtonMonitor(){

    if(normal)
        glfwDestroyCursor(normal);

    if(active)
        glfwDestroyCursor(active);

}

EventInfo MouseButtonMonitor::Query(int button){

    assert(window != nullptr && "OpenGL context can't be null");

    EventInfo info = {};

    double mouseX, mouseY;
    // Read current mouse position and window size
    glfwGetCursorPos(window, &mouseX, &mouseY);
    glfwGetWindowSize(window, &width, &height);

    // Apply vertical correction
    mouseY = height-mouseY;

    if( button >= GLFW_MOUSE_BUTTON_LAST)
        return info;

    // Check if mouse position is in window boundary
    bool inWindow = (mouseX > 0.0f && mouseX <= width && mouseY > 0.0f && mouseY <= height);

    // Check if the mouse has moved
    bool positionChanged = (mouseX != lastXPosition || mouseY != lastYPosition);

    int currentState = glfwGetMouseButton(window, button);

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
