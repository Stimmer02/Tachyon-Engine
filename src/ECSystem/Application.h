#ifndef APPLICATION_H
#define APPLICATION_H

#include "WindowContext.h"
#include "GraphicSystem.h"
#include "InteractionManager.h"
#include "Configurator.h"
#include "Input.h"
#include "Timer.h"

class Application{

private:

    WindowContext context;
    ILog * contextLogger;
    Input * inputInstance;

    GraphicSystem * graphics;
    std::list<System *> systems;

    InteractionManager interactionManager;
    SharedNameResolver resourceManager;

    Timer * timer;

    void LoadConfiguration(){

        Configurator configurator("settings.conf");

        // Graphic settings

        configurator.ParseString("title", GraphicConfig::windowTitle, "Window");
        configurator.ParseInt("width", GraphicConfig::windowWidth, 800);
        configurator.ParseInt("height", GraphicConfig::windowHeight, 600);
        configurator.ParseBoolean("visible", GraphicConfig::visible, true);
        configurator.ParseBoolean("resizable", GraphicConfig::resiazble, true);
        configurator.ParseBoolean("vsync", GraphicConfig::vsync, true);
        configurator.ParseBoolean("zbuffer", GraphicConfig::zbuffer, true);
        configurator.ParseBoolean("ortho", GraphicConfig::useOrthographicProjection, true);

        // Logic settings

        configurator.ParseBoolean("internalinteraction", ApplicationConfig::internalGUIInteraction, true);

    }

public:

    Application(){

        LoadConfiguration();

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

        resourceManager.Emplace("scene", &scene, sizeof(scene));
        graphics->LoadScene( &scene );
    }

    void RegisterSystem(System * system){
        contextLogger->Write(LogMessageType::M_INFO, "New system registered\n");

        system->Share(&resourceManager);

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

        contextLogger->Write(M_INFO, "Sharing resources");

        for(System * system : systems)
            system->Share(&resourceManager);

        contextLogger->Write(M_INFO, "Preparing %d systems", systems.size());

        for( System * system : systems)
            system->OnUnload();

        contextLogger->Write(M_INFO, "Executing main loop");

        while( !context.ShouldClose() ){

            timer->TicTac();
            inputInstance->Update();

            for( System * system : systems)
                system->Run();

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

        contextLogger->Write(M_INFO, "Main loop aborted");

        for( System * system : systems)
            system->OnUnload();

    }

    ~Application(){

        context.Close();

        contextLogger->Write(LogMessageType::M_INFO, "Disposing resources\n");
        contextLogger->Write(LogMessageType::M_INFO, "Unmanaging context\n");

        delete graphics;

        contextLogger->Flush();
    }

};

#endif
