#ifndef INPUT_H
#define INPUT_H

#include "EventInfo.h"
#include "Vector3.h"
#include "WindowContext.h"

class Input{

private:

    WindowContext * context;

    double lastXPosition, lastYPosition;

    char lastMouseStates[GLFW_MOUSE_BUTTON_LAST + 1];
    char lastKeyboardStates[GLFW_KEY_LAST];

public:


    EventType GetKeyState(const int & key){

        EventType type = EventType::NONE;

        if( key >= GLFW_KEY_LAST)
            return type;

        int currentState = context->GetKeyboardKey(key);

        // Check if the key is held
        bool isButtonHeld = (currentState == GLFW_PRESS) && (lastKeyboardStates[key] == GLFW_PRESS);

        // Check if the key is triggered
        bool isButtonTriggered = (currentState == GLFW_PRESS) && (lastKeyboardStates[key] == GLFW_RELEASE);

        // Check if the key is released
        bool isButtonReleased = (currentState == GLFW_RELEASE) && (lastKeyboardStates[key] == GLFW_PRESS);

        if(isButtonTriggered){
            type = EventType::ONTRIGGER;
        }else if(isButtonReleased){
            type = EventType::ONRELEASE;
        }else if (isButtonHeld){
            type = EventType::ONHOLD;
        }else{
            type = EventType::NONE;
        }

        lastKeyboardStates[key] = currentState;

        return type;
    }

    Vector3 GetMousePosition(){
        return Vector3(lastXPosition, lastYPosition);
    }

    EventType GetButtonState(const int & button){

        int width, height;
        double mouseX, mouseY;

        EventType type = EventType::NONE;

        // Read current mouse position and window size
        context->GetMousePos(mouseX, mouseY);
        context->GetWindowSize(width, height);

        // Apply vertical correction
        mouseY = height-mouseY;

        if( button >= GLFW_MOUSE_BUTTON_LAST)
            return NONE;

        // Check if mouse position is in window boundary
        bool inWindow = (mouseX > 0.0f && mouseX <= width && mouseY > 0.0f && mouseY <= height);

        // Check if the mouse has moved
        bool positionChanged = (mouseX != lastXPosition || mouseY != lastYPosition);

        int currentState = context->GetMouseButton(button);

        // Check if the mouse button is triggered
        bool isButtonHeld = (currentState == GLFW_PRESS) && (lastMouseStates[button] == GLFW_PRESS);

        // Check if the mouse button is triggered
        bool isButtonTriggered = (currentState == GLFW_PRESS) && (lastMouseStates[button] == GLFW_RELEASE);

        // Check if the mouse button is released
        bool isButtonReleased = (currentState == GLFW_RELEASE) && (lastMouseStates[button] == GLFW_PRESS);

        if(positionChanged){
            type = EventType::ONMOVE;
        }else if(isButtonTriggered){
            type = EventType::ONTRIGGER;
        }else if(isButtonReleased){
            type = EventType::ONRELEASE;
        } else if (isButtonHeld){
            type = EventType::ONHOLD;
        }else if(inWindow){
            type = EventType::ONHOVER;
        }else{
            type = EventType::NONE;
        }

        lastMouseStates[button] = currentState;

        lastXPosition = mouseX;
        lastYPosition = mouseY;

        return type;
    }

    void SetContext( WindowContext * context ){
        assert(context != nullptr && "Context cannot be null");
        this->context = context;
    }

    static Input& GetInstance(){
        static Input instance;
        return instance;
    }

};


#endif
