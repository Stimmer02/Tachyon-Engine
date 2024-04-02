#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "System.h"
#include <functional>
#include "WindowContext.h"

using RenderFunc = std::function<void()>;

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

        context.CreateWindow();
        context.SetVSync();
        this->renderFunc = delegate;

    }

    void Share() override{

    }

    ~GraphicSystem(){

    }
};



#endif
