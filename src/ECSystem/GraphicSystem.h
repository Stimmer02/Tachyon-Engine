#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "System.h"
#include "WindowContext.h"

class GraphicSystem : public System{
private:

    WindowContext context;

    void OnLoad() override{
        context.CreateWindow();
        context.SetVSync();
    }

    void OnUnload() override{

    }

    void Execute() override{

        if ( context.ShouldClose() ){
            this->Stop();
        }

        context.PoolEvents();
        context.SwapBuffers();
        context.CheckErrors();
    }

public:
    GraphicSystem() : System(){

    }

    void Share() override{

    }

    ~GraphicSystem(){

    }
};



#endif
