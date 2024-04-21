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

    std::list<System *> systems;

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

    void LoadScene(Scene & scene){
        graphics->LoadScene( &scene );
    }

    void RegisterSystem(System * system){
        systems.emplace_back(system);
    }

    void RemoveSystem(System * system){

        for( std::list<System*>::iterator it = systems.begin(); it != systems.end(); it++){
            if( *it == system ){
                systems.erase(it);
                break;
            }
        }

    }

    MouseButtonMonitor& GetMouseInputMonitor() {
        return mouseMonitor;
    }

    KeyboardMonitor& GerKeyboardInputMonitor() {
        return keyboardMonitor;
    }

    void Loop(){

        while( !context.ShouldClose() ){

            timer->TicTac();
            graphics->Run();

            for(System * system : systems){
                system->Run();
            }

            if(timer->GetAccumulatedTime()>= 1.0f){
                fprintf(stdout, "FPS : %05d\r", timer->GetFrameCount());
                fflush(stdout);
            }

        }

    }

    ~Application(){
        delete graphics;
    }


};

#endif
