#ifndef INPUT_H
#define INPUT_H

#include "EventInfo.h"
#include "Vector3.h"
#include "WindowContext.h"

class Input{

private:

    WindowContext * context;

    double lastXPosition, lastYPosition;

    EventType lastMouseStates[GLFW_MOUSE_BUTTON_LAST + 1];
    EventType lastKeyboardStates[GLFW_KEY_LAST + 1];

    void UpdateKey(const int & key){

        EventType type = EventType::NONE;

        int currentState = context->GetKeyboardKey(key);

        // Check if the key is held
        bool isButtonHeld = (currentState == GLFW_PRESS);

        // Check if the key is triggered
        bool isButtonTriggered = (currentState == GLFW_PRESS) && (lastKeyboardStates[key] == EventType::ONRELEASE);

        // Check if the key is released
        bool isButtonReleased = (currentState == GLFW_RELEASE) && (lastKeyboardStates[key] != EventType::NONE);

        if(isButtonTriggered){
            type = EventType::ONTRIGGER;
        } else if(isButtonReleased){
            type = EventType::ONRELEASE;
        } else if(isButtonHeld){
            type = EventType::ONHOLD;
        } else {
            type = EventType::NONE;
        }

        lastKeyboardStates[key] = type;
    }

    void UpdateButton(const int & button){

        int width, height;
        double mouseX, mouseY;

        EventType type = EventType::NONE;

        // Read current mouse position and window size
        context->GetMousePos(mouseX, mouseY);
        context->GetWindowSize(width, height);

        // Apply vertical correction
        mouseY = height-mouseY;

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

        lastMouseStates[button] = type;

        lastXPosition = mouseX;
        lastYPosition = mouseY;

    }

    Input(){

        for(int key=0; key<=GLFW_KEY_LAST; key++)
            lastKeyboardStates[key] = ONRELEASE;

        for(int button=0; button<=GLFW_MOUSE_BUTTON_LAST; button++)
            lastMouseStates[button] = ONRELEASE;

    }

public:

    void Update(){

        for(int key=0; key<=GLFW_KEY_LAST; key++)
            UpdateKey(key);

        for(int button=0; button<=GLFW_MOUSE_BUTTON_LAST; button++)
            UpdateButton(button);

    }

    EventType GetKeyState(const int & key){

        if( key >= GLFW_KEY_LAST)
            return NONE;

        return lastKeyboardStates[key];
    }

    Vector3 GetMousePosition(){
        return Vector3(lastXPosition, lastYPosition);
    }

    EventType GetButtonState(const int & button){

        if( button >= GLFW_MOUSE_BUTTON_LAST)
            return NONE;

        return lastMouseStates[button];
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
