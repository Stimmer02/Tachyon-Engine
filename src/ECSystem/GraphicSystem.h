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
    GLShader * mainShader;

    Matrix projectionMatrix;

    void Execute() override{
        context->PoolEvents();
        context->SwapBuffers();
        context->CheckErrors();

        if(!scene)
            return;

        mainShader->Use();
        mainShader->TransferToShader("u_projection", projectionMatrix);

        std::pair<IterationItem, IterationItem> iterator = scene->GetSceneObjectsIterator();

        for(IterationItem it = iterator.first; it != iterator.second; it++){

            SceneObject * object = *it;

            if (object == nullptr || object->GetActivity() == false)
                continue;

            object->Render();
            object->Update(); // It should be somewhere elese

        }

        mainShader->Dispose();

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

        this->projectionMatrix = MatrixUtils::Ortho(0, GraphicConfig::windowWidth, 0, GraphicConfig::windowHeight, -10, 10);

        this->mainShader = new GLShader();
        this->mainShader->LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
        this->mainShader->LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
        this->mainShader->Build();

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
