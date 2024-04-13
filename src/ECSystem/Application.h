#ifndef APPLICATION_H
#define APPLICATION_H

#include "WindowContext.h"
#include "GraphicSystem.h"
#include "MouseButtonMonitor.h"
#include "KeyboardMonitor.h"
#include "Timer.h"

class Application{

private:

    WindowContext context;
    GraphicSystem * graphics;
    MouseButtonMonitor mouseMonitor;
    KeyboardMonitor keyboardMonitor;
    Timer * timer;

public:

    Application(){
        this->graphics = new GraphicSystem(&context);
        this->mouseMonitor = MouseButtonMonitor(&context);
        this->keyboardMonitor = KeyboardMonitor(&context);
        this->timer = &Timer::GetInstance();
    }

    void SetRenderFunc(RenderFunc func){
        graphics->SetRenderFunc(func);
    }

    MouseButtonMonitor& GetMouseInputMonitor(){
        return mouseMonitor;
    }

    KeyboardMonitor& GerKeyboardInputMonitor(){
        return keyboardMonitor;
    }

    void Loop(){

        while( !context.ShouldClose() ){

            graphics->Run();
            timer->TicTac();

        }

    }

    ~Application(){
        delete graphics;
    }


};

#endif
