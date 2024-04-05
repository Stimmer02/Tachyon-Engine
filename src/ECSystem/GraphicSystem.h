#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "System.h"
#include <functional>
#include "WindowContext.h"

using RenderFunc = std::function<void()>;

namespace GraphicConfig{
    const char * windowTitle = "Window";
    int32_t windowWidth = 800;
    int32_t windowHeight = 600;
    bool vsync = true;
    bool zbuffer = true;
};

class GraphicSystem : public System{
private:

    WindowContext context;

    RenderFunc renderFunc;

    void OnLoad() override{
        
    }

    void OnUnload() override{

    }

    void Execute() override{

        if( context.ShouldClose() )
            this->Stop();

        renderFunc();

        context.PoolEvents();
        context.SwapBuffers();
        context.CheckErrors();
    }

public:

    GraphicSystem(const RenderFunc & delegate) : System(){

        context.CreateWindow(GraphicConfig::windowWidth, GraphicConfig::windowHeight, GraphicConfig::windowTitle);
        context.SetVSync( GraphicConfig::vsync );
        context.SetZBuffer( GraphicConfig::zbuffer );

        this->renderFunc = delegate;

    }

    void Share() override{

    }

    ~GraphicSystem(){

    }
};



#endif
