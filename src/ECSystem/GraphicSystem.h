#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "AttributeContainer.h"
#include "TransformStack.h"
#include "System.h"
#include "Sprite.h"
#include "WindowContext.h"
#include "Settings.h"

#include <functional>

using RenderFunc = std::function<void()>;

class GraphicSystem : public System{
private:

    WindowContext * context;

    AttributeContainer<Sprite> textures;

    void OnLoad() override{

    }

    void OnUnload() override{

    }

    void Execute() override{
        context->PoolEvents();
        context->SwapBuffers();
        context->CheckErrors();
    }

public:

    GraphicSystem(WindowContext * context) : System(){

        this->context = context;

        context->CreateWindow(GraphicConfig::windowWidth, GraphicConfig::windowHeight, GraphicConfig::windowTitle);
        context->SetVSync( GraphicConfig::vsync );
        context->SetZBuffer( GraphicConfig::zbuffer );

        TransformStack::Initialize();

    }

    void Share() override{

    }

    ~GraphicSystem(){

    }
};



#endif
