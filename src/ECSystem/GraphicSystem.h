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

using ArchetypeRenderFunc = std::function< void(SceneObject *) >;

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
    Matrix guiProjectionMatrix;

    ArchetypeRenderFunc archetypeFunc[RenderingAttributes::ATTRIB_MAX];

    void RenderSceneObjects(SceneObject * object){

        if(object == nullptr || object->isActive == false)
            return;

        Matrix & model = object->GetModel();
        currentShader->TransferToShader("u_model", model);

        Archetype archetype = object->GetArchetype() & (RenderingAttributes::ATTRIB_MAX - 1);

        archetypeFunc[archetype](object);

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

        std::vector<SceneObject *>& objects = scene->GetSceneObjects();
        std::vector<GUIElement *>& guiElements = scene->GetGUIElements();

        for(SceneObject * object : objects)
            RenderSceneObjects(object);

        mainShader->Dispose();

        guiShader->Use();
        guiShader->TransferToShader("u_projection", guiProjectionMatrix);

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
        this->guiShader->LinkShader("./resources/shaders/guiVertexShader.vert", GL_VERTEX_SHADER);
        this->guiShader->LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
        this->guiShader->Build();
    }

    void SetupArchetypeFunc(){

        contextLogger->Write(LogMessageType::M_INFO, "Building rendering archetypes\n");

        archetypeFunc[RenderingAttributes::NONEATTRIB] =
            [this](SceneObject * object){
                return ;
            };

        archetypeFunc[RenderingAttributes::SPRITE] =
            [this](SceneObject * object){

                Sprite * sprite = object->GetAttribute<Sprite>();
                Mesh * mesh = this->GetDefaultMesh();

                sprite->Load();
                mesh->Draw();
            };

        archetypeFunc[RenderingAttributes::MESH] =
            [this](SceneObject * object){

                Sprite * sprite = this->GetDefaultTexture();
                Mesh * mesh = object->GetAttribute<Mesh>();

                sprite->Load();
                mesh->Draw();
            };

        archetypeFunc[RenderingAttributes::LINE] =
            [](SceneObject * object){

            };

    }

public:

    GraphicSystem(WindowContext * context) : System(){

        this->context = context;
        this->contextLogger = context->GetContextLogger();

        context->Open(GraphicConfig::windowWidth, GraphicConfig::windowHeight, GraphicConfig::windowTitle);
        context->SetVSync( GraphicConfig::vsync );
        context->SetZBuffer( GraphicConfig::zbuffer );

        if(GraphicConfig::useOrthographicProjection){
            this->projectionMatrix = Matrix::Ortho(0, GraphicConfig::windowWidth, 0, GraphicConfig::windowHeight, -300, 300);
        }else{
            this->projectionMatrix = Matrix::Frustum(0, GraphicConfig::windowWidth, 0, GraphicConfig::windowHeight, -100, 100);
        }

        this->guiProjectionMatrix = Matrix::Ortho(0, GraphicConfig::windowWidth, 0, GraphicConfig::windowHeight, -1, 10);

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
