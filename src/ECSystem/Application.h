#ifndef APPLICATION_H
#define APPLICATION_H

#include "WindowContext.h"
#include "GraphicSystem.h"
#include "InteractionManager.h"
#include "MouseButtonMonitor.h"
#include "KeyboardMonitor.h"
#include "Timer.h"

class Application{

private:

    WindowContext context;
    ILog * contextLogger;

    GraphicSystem * graphics;
    MouseButtonMonitor * mouseMonitor;
    KeyboardMonitor * keyboardMonitor;

    InteractionManager interactionManager;

    std::list<System *> systems;

    Timer * timer;

public:

    Application(){

        this->graphics = new GraphicSystem(&context);
        this->contextLogger = context.GetContextLogger();

        contextLogger->Write(LogMessageType::M_INFO, "Creating new mouse state monitor\n");
        this->mouseMonitor = new MouseButtonMonitor(&context);

        contextLogger->Write(LogMessageType::M_INFO, "Creating new keyboard state monitor\n");
        this->keyboardMonitor = new KeyboardMonitor(&context);

        this->timer = &Timer::GetInstance();
        this->systems.push_back(graphics);

    }

    void LoadScene(Scene & scene){
        contextLogger->Write(LogMessageType::M_INFO, "Loading new scene\n");

        if( ApplicationConfig::internalGUIInteraction ){
            contextLogger->Write(LogMessageType::M_INFO, "Enabling internal interaction handling\n");
            interactionManager.LoadScene(scene);
        }

        graphics->LoadScene( &scene );
    }

    void RegisterSystem(System * system){
        contextLogger->Write(LogMessageType::M_INFO, "New system registered\n");
        systems.emplace_back(system);
    }

    void RemoveSystem(System * system){

        for( std::list<System*>::iterator it = systems.begin(); it != systems.end(); it++){
            if( *it == system ){
                contextLogger->Write(LogMessageType::M_INFO, "Erasing system data\n");
                systems.erase(it);
                break;
            }
        }

    }

    MouseButtonMonitor& GetMouseInputMonitor() {
        return *mouseMonitor;
    }

    KeyboardMonitor& GetKeyboardInputMonitor() {
        return *keyboardMonitor;
    }

    Camera& GetMainCamera(){
        return graphics->GetMainCamera();
    }

    void Loop(){

        while( !context.ShouldClose() ){

            timer->TicTac();

            for(System * system : systems){
                system->Run();
            }

            EventInfo leftMouseButtonEvent = mouseMonitor->GeyKeyState(GLFW_MOUSE_BUTTON_LEFT);

            if( ( leftMouseButtonEvent.type == ONTRIGGER )  && ApplicationConfig::internalGUIInteraction)
                interactionManager.Interact(leftMouseButtonEvent.x, leftMouseButtonEvent.y);

            if(timer->GetAccumulatedTime()>= 1.0f){
                fprintf(stdout, "FPS : %05d\r", timer->GetFrameCount());
                fflush(stdout);
            }

        }

    }

    ~Application(){

        contextLogger->Write(LogMessageType::M_INFO, "Disposing resources\n");

        delete graphics;
        delete mouseMonitor;
        delete keyboardMonitor;
    }

};

#endif
