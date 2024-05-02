#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "System.h"
#include "WindowContext.h"
#include "GLShader.h"
#include "Scene.h"

#include "Transform.h"
#include "Sprite.h"
#include "Mesh.h"

#include "TextElement.h"
#include "ButtonElement.h"

#include "Camera.h"

#include <functional>

using ArchetypeRenderFunc = std::function< void(Sprite *, Mesh *) >;

class GraphicSystem : public System{
private:

    Scene * scene;
    WindowContext * context;

    Camera mainCamera;

    GLShader * mainShader;
    GLShader * guiShader;

    Mesh * defaultMesh;

    ILog * contextLogger;

    Matrix projectionMatrix;

    ArchetypeRenderFunc archetypeFunc[RenderingAttributes::ATTRIB_MAX];

    void RenderSceneObjects(SceneObject * object){

        if(object == nullptr || object->isActive == false)
            return;

        Sprite * texture = object->GetAttribute<Sprite>();
        Mesh * mesh = object->GetAttribute<Mesh>();

        Matrix & model = object->GetModel();
        currentShader->TransferToShader("u_model", model);

        Archetype archetype = object->GetArchetype();
        archetype &= RenderingAttributes::SPRITEANDMESH;

        archetypeFunc[archetype](texture, mesh);

        for( SceneObject * children : object->GetChildrens() )
            RenderSceneObjects(children);

        object->Update();

    }

    void RenderGUI(GUIElement * guiElement){

        if(guiElement == nullptr || guiElement->isActive == false)
            return;

        guiElement->Render();

    }

    void Execute() override{
        context->PoolEvents();
        context->SwapBuffers();
        context->CheckErrors();

        if(!scene)
            return;

        mainShader->Use();
        mainShader->TransferToShader("u_projection", projectionMatrix);
        mainShader->TransferToShader("u_view", mainCamera.GetViewMatrix());

        std::list<SceneObject *>& objects = scene->GetSceneObjects();
        std::list<GUIElement *>& guiElements = scene->GetGUIElements();

        for(SceneObject * object : objects)
            RenderSceneObjects(object);

        mainShader->Dispose();

        guiShader->Use();
        guiShader->TransferToShader("u_projection", projectionMatrix);

        for(GUIElement * guiElement : guiElements)
            RenderGUI(guiElement);

        guiShader->Dispose();

    }

    void SetupUnhandledComponents() {

        contextLogger->Write(LogMessageType::M_INFO, "Generating missing assets\n");

        Color white[] = {255,255,255};
        defaultSprite = new Sprite(white, 1, 1);

        defaultMesh = new Mesh();
        defaultMesh->GenQuad(1.0f, 1.0f);

    }

    void UploadMainShaders() {

        contextLogger->Write(LogMessageType::M_INFO, "Compiling shaders\n");

        this->mainShader = new GLShader();
        this->mainShader->LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
        this->mainShader->LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
        this->mainShader->Build();

        this->guiShader = new GLShader();
        this->guiShader->LinkShader("./resources/shaders/textVertexShader.vert", GL_VERTEX_SHADER);
        this->guiShader->LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
        this->guiShader->Build();
    }

    void SetupArchetypeFunc(){

        contextLogger->Write(LogMessageType::M_INFO, "Building rendering archetypes\n");

        archetypeFunc[RenderingAttributes::NONEATTRIB] =
            [this](Sprite * sprite, Mesh * mesh){

                sprite = this->GetDefaultTexture();
                mesh = this->GetDefaultMesh();

                sprite->Load();
                mesh->Draw();
                sprite->UnLoad();
            };

        archetypeFunc[RenderingAttributes::SPRITE] =
            [this](Sprite * sprite, Mesh * mesh){

                mesh = this->GetDefaultMesh();

                sprite->Load();
                mesh->Draw();
                sprite->UnLoad();
            };

        archetypeFunc[RenderingAttributes::MESH] =
            [this](Sprite * sprite, Mesh * mesh){

                sprite = this->GetDefaultTexture();

                sprite->Load();
                mesh->Draw();
                sprite->UnLoad();
            };

        archetypeFunc[RenderingAttributes::SPRITEANDMESH] =
            [this](Sprite * sprite, Mesh * mesh){

                sprite->Load();
                mesh->Draw();
                sprite->UnLoad();
            };

    }

public:

    GraphicSystem(WindowContext * context) : System(){

        this->context = context;
        this->contextLogger = context->GetContextLogger();

        context->Open(GraphicConfig::windowWidth, GraphicConfig::windowHeight, GraphicConfig::windowTitle);
        context->SetVSync( GraphicConfig::vsync );
        context->SetZBuffer( GraphicConfig::zbuffer );

        this->projectionMatrix = Matrix::Ortho(0, GraphicConfig::windowWidth, 0, GraphicConfig::windowHeight, -100, 100);

        SetupUnhandledComponents();
        UploadMainShaders();
        SetupArchetypeFunc();
    }

    void LoadScene(Scene * scene){
        this->scene = scene;
    }

    void Share() override{

    }

    Camera& GetMainCamera(){
        return mainCamera;
    }

    Sprite * GetDefaultTexture(){
        return defaultSprite;
    }

    Mesh * GetDefaultMesh(){
        return defaultMesh;
    }

    ~GraphicSystem(){
        delete defaultSprite;
        delete defaultMesh;
    }
};



#endif
