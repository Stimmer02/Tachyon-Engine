#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "TransformStack.h"
#include "System.h"
#include "WindowContext.h"
#include "Settings.h"
// #include "Scene.h"

#include <functional>

using RenderFunc = std::function<void()>;

class GraphicSystem : public System{
private:

    //Scene * scene;
    WindowContext * context;

    RenderFunc func;

    void OnLoad() override{

    }

    void OnUnload() override{

    }

    void Execute() override{
        context->PoolEvents();
        context->SwapBuffers();
        context->CheckErrors();

        // if(scene)
        //     scene->Render();

        // TODO : remove
        if(func)
            func();

    }

public:

    GraphicSystem(WindowContext * context) : System(){

        this->context = context;

        context->Open(GraphicConfig::windowWidth, GraphicConfig::windowHeight, GraphicConfig::windowTitle);
        context->SetVSync( GraphicConfig::vsync );
        context->SetZBuffer( GraphicConfig::zbuffer );

        TransformStack::Initialize();

    }

    void SetRenderFunc(RenderFunc & func){
        this->func = func;
    }

    // void LoadScene(Scene * scene){
    //     this->scene = scene;
    // }

    void Share() override{

    }

    ~GraphicSystem(){

    }
};



#endif
