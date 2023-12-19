#include "MouseInputService.h"

MouseInputService::MouseInputService(const GLFWwindow * _window){
    this->window = (GLFWwindow*)_window;
}

void MouseInputService::BindWindow(GLFWwindow * _window){
    this->window = _window;

    // Enable custom cursor within current window scope
    if(normal)
        glfwSetCursor(window, normal);

}

void MouseInputService::SetNormalCursor(const unsigned char* pixels, const size_t & width, const size_t & height){

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

void MouseInputService::SetActiveCursor(const unsigned char* pixels, const size_t & width, const size_t & height){

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

MouseInputService::~MouseInputService(){

    if(normal)
        glfwDestroyCursor(normal);

    if(active)
        glfwDestroyCursor(active);

}

EventInfo MouseInputService::Query(){
    EventInfo info;

    // No event happened
    info.type = EventType::ONHOVER;

    // Read current mouse position and window size
    glfwGetCursorPos(window, &info.x, &info.y);
    glfwGetWindowSize(window, &width, &height);

    // Apply vertical correction
    info.y=height-info.y;

    // Check if mouse position is in window boundary
    bool isInBound = (info.x > 0.0f && info.x <= width && info.y > 0.0f && info.y <= height);

    // Check if the mouse has moved
    bool isFocused = (lastXPosition != info.x || lastYPosition != info.y);

    // Check if the left mouse button is pressed
    bool isButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // Calculate an index
    int conditionIndex = isFocused + (isButtonPressed << 1);

    // Detrmine if event can be recognized within window scope
    conditionIndex*=isInBound;

    // Zero position if mouse position points outside the window
    info.x*=isInBound;
    info.y*=isInBound;

    // Cast the event type using the index
    info.type = (EventType)(conditionIndex);

    // Remember last mouse position
    lastXPosition = info.x;
    lastYPosition = info.y;

    return info;
}
