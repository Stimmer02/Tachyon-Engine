#ifndef APPLICATION_H
#define APPLICATION_H

#define GL_SILENCE_DEPRECATION

#pragma once

#include "WindowContext.h"
#include "GraphicSystem.h"
#include "InteractionManager.h"
#include "Configurator.h"
#include "Input.h"
#include "Timer.h"

class Application{

private:

    WindowContext context;
    Timer timer;

    ILog * contextLogger;
    Input * inputInstance;

    GraphicSystem * graphics;
    std::list<System *> systems;

    InteractionManager interactionManager;
    SharedNameResolver resourceManager;

    void LoadConfiguration(){

        Configurator configurator("settings.cfg");

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
        configurator.ParseString("physicProcessorConfigPath", ApplicationConfig::physicProcessorConfig, "./config/system.cfg");
    }

public:

    Application(){

        LoadConfiguration();

        this->graphics = new GraphicSystem();
        this->contextLogger = context.GetContextLogger();

        this->resourceManager.Emplace("context", &context, sizeof(WindowContext));
        this->resourceManager.Emplace("PPConfigPath", &ApplicationConfig::physicProcessorConfig, sizeof(char) * ApplicationConfig::physicProcessorConfig.size());

        contextLogger->Write(M_INFO, "Initializing I/O handles");
        this->inputInstance = &Input::GetInstance();
        this->inputInstance->SetContext(&context);

        contextLogger->Write(M_INFO, "Instancing internal timer");
        resourceManager.Emplace("timer", &timer, sizeof(Timer));

        resourceManager.Emplace("application", this, sizeof(Application));

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

    void Loop(){

        contextLogger->Write(M_INFO, "Sharing resources");

        for(System * system : systems)
            system->Share(&resourceManager);

        contextLogger->Write(M_INFO, "Loading systems");

        for( System * system : systems)
            system->OnLoad();

        contextLogger->Write(M_INFO, "Executing main loop");

        while( !context.ShouldClose() ){

            timer.TicTac();
            inputInstance->Update();

            for( System * system : systems)
                system->Run();

            EventType leftMouseButtonEvent = inputInstance->GetKeyState(GLFW_MOUSE_BUTTON_LEFT);

            if( ( leftMouseButtonEvent == ONTRIGGER )  && ApplicationConfig::internalGUIInteraction ){
                Vector3 mousePosition = inputInstance->GetMousePosition();
                interactionManager.Interact(mousePosition.x, mousePosition.y);
            }

            if(timer.GetAccumulatedTime()>= 1.0f){
                fprintf(stdout, "FPS : %05d\r", timer.GetFrameCount());
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

        delete graphics;

        contextLogger->Flush();
    }

};

#endif
