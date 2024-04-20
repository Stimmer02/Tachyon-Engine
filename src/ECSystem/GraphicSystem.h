#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "TransformStack.h"
#include "System.h"
#include "WindowContext.h"
#include "Settings.h"
#include "GLShader.h"
#include "Scene.h"


class GraphicSystem : public System{
private:

    Scene * scene;
    WindowContext * context;

    void Execute() override{
        context->PoolEvents();
        context->SwapBuffers();
        context->CheckErrors();

        if(!scene)
            return;

        std::pair<IterationItem, IterationItem> iterator = scene->GetSceneObjectsIterator();

        for(IterationItem it = iterator.first; it != iterator.second; it++){

            SceneObject * object = *it;

            if (object == nullptr || object->GetActivity() == false)
                continue;

            object->Render();
            object->Update();

        }

    }

    void OnLoad() override{


    }

    void OnUnload() override{
    }

public:

    GraphicSystem(WindowContext * context) : System(){

        this->context = context;

        context->Open(GraphicConfig::windowWidth, GraphicConfig::windowHeight, GraphicConfig::windowTitle);
        context->SetVSync( GraphicConfig::vsync );
        context->SetZBuffer( GraphicConfig::zbuffer );


        TransformStack::Initialize();

    }

    void LoadScene(Scene * scene){
        this->scene = scene;
    }

    void Share() override{

    }

    ~GraphicSystem(){

    }
};



#endif
