#include "KeyboardMonitor.h"

KeyboardMonitor::KeyboardMonitor(){

}

KeyboardMonitor::KeyboardMonitor(WindowContext * context){
    this->window = context;
    memset(lastKeyboardStates, GLFW_RELEASE, sizeof(char) * GLFW_MOUSE_BUTTON_LAST);
}

EventInfo KeyboardMonitor::GetButtonState(int button){

    assert(window != nullptr && "OpenGL context can't be null");

    EventInfo info = {};

    if( button >= GLFW_KEY_LAST)
        return info;

    int currentState = window->GetKeyboardKey(button);

    // Check if the key is held
    bool isButtonHeld = (currentState == GLFW_PRESS) && (lastKeyboardStates[button] == GLFW_PRESS);

    // Check if the key is triggered
    bool isButtonTriggered = (currentState == GLFW_PRESS) && (lastKeyboardStates[button] == GLFW_RELEASE);

    // Check if the key is released
    bool isButtonReleased = (currentState == GLFW_RELEASE) && (lastKeyboardStates[button] == GLFW_PRESS);

    if(isButtonTriggered){
        info.type = EventType::ONTRIGGER;
    }else if(isButtonReleased){
        info.type = EventType::ONRELEASE;
    }else if (isButtonHeld){
        info.type = EventType::ONHOLD;
    }else{
        info.type = EventType::NONE;
    }

    lastKeyboardStates[button] = currentState;

    return info;
}

KeyboardMonitor::~KeyboardMonitor(){

}
