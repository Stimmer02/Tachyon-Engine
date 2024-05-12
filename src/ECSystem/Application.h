#ifndef APPLICATION_H
#define APPLICATION_H

#include "WindowContext.h"
#include "GraphicSystem.h"
#include "InteractionManager.h"
#include "Input.h"
#include "Timer.h"

class Application{

private:

    WindowContext context;
    ILog * contextLogger;
    Input * inputInstance;

    GraphicSystem * graphics;

    InteractionManager interactionManager;

    std::list<System *> systems;

    Scene * scene;

    Timer * timer;

public:

    Application(){

        this->graphics = new GraphicSystem(&context);
        this->contextLogger = context.GetContextLogger();

        contextLogger->Write(M_INFO, "Initializing I/O handles");
        this->inputInstance = &Input::GetInstance();
        this->inputInstance->SetContext(&context);

        contextLogger->Write(M_INFO, "Instancing internal timer");
        this->timer = &Timer::GetInstance();

        contextLogger->Write(M_INFO, "Pushing graphic system to systems pool");
        this->systems.push_back(graphics);

    }

    void LoadScene(Scene & scene){
        contextLogger->Write(LogMessageType::M_INFO, "Loading new scene\n");

        if( ApplicationConfig::internalGUIInteraction ){
            contextLogger->Write(LogMessageType::M_INFO, "Enabling internal interaction handling\n");
            interactionManager.LoadScene(scene);
        }

        this->scene = &scene;
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

    Camera& GetMainCamera(){
        return graphics->GetMainCamera();
    }

    void Loop(){

        while( !context.ShouldClose() ){

            timer->TicTac();
            inputInstance->Update();

            for(System * system : systems){
                system->Run();
            }

            EventType leftMouseButtonEvent = inputInstance->GetKeyState(GLFW_MOUSE_BUTTON_LEFT);

            if( ( leftMouseButtonEvent == ONTRIGGER )  && ApplicationConfig::internalGUIInteraction){
                Vector3 mousePosition = inputInstance->GetMousePosition();
                interactionManager.Interact(mousePosition.x, mousePosition.y);
            }

            if(timer->GetAccumulatedTime()>= 1.0f){
                fprintf(stdout, "FPS : %05d\r", timer->GetFrameCount());
                fflush(stdout);
                contextLogger->Flush();
            }

        }

    }

    ~Application(){

        contextLogger->Write(LogMessageType::M_INFO, "Disposing resources\n");
        contextLogger->Write(LogMessageType::M_INFO, "Unmanaging context\n");

        delete graphics;

        contextLogger->Flush();
    }

};

#endif
