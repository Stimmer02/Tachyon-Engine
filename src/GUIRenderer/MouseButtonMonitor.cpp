#include "MouseButtonMonitor.h"

MouseButtonMonitor::MouseButtonMonitor(){
    this->window = nullptr;
    this->normal = nullptr;
    this->active = nullptr;

    memset(lastStates, GLFW_RELEASE, sizeof(int) * GLFW_MOUSE_BUTTON_LAST);

}

void MouseButtonMonitor::AcceptGLFWContext(GLFWwindow * _window){
    this->window = _window;

    // Enable custom cursor within current window scope
    if( normal != nullptr )
        glfwSetCursor(window, normal);

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

void MouseButtonMonitor::Update(){

    // Update all states
    for(int i=0; i < GLFW_MOUSE_BUTTON_LAST; i++)
        lastStates[i] = glfwGetMouseButton(window, i);

}

EventInfo MouseButtonMonitor::Query(int button){
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
    info.withinWindow = (mouseX > 0.0f && mouseX <= width && mouseY > 0.0f && mouseY <= height);

    // Check if the mouse has moved
    bool positionChanged = (mouseX != lastXPosition || mouseY != lastYPosition);

    int currentState = glfwGetMouseButton(window, button);

    // Check if the mouse button is triggered
    bool isButtonTriggered = (currentState == GLFW_PRESS) && (lastStates[button] == GLFW_RELEASE);

    // Check if the mouse button is released
    bool isButtonReleased = (currentState == GLFW_RELEASE) && (lastStates[button] == GLFW_PRESS);

    if(positionChanged){
        info.type = EventType::ONMOVE;
    }else if(isButtonTriggered){
        info.type = EventType::ONTRIGGER;
    }else if(isButtonReleased){
        info.type = EventType::ONRELEASE;
    } else {
        info.type = (info.withinWindow) ? EventType::ONHOVER : EventType::NONE;
    }

    info.x = mouseX;
    info.y = mouseY;

    // Remember last mouse position
    lastXPosition = info.x;
    lastYPosition = info.y;

    return info;
}
