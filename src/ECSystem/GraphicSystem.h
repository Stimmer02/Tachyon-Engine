#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "System.h"
#include "WindowContext.h"
#include "GLShader.h"
#include "Scene.h"

#include "Transform.h"
#include "LineRenderer.h"
#include "Sprite.h"
#include "Mesh.h"

#include "TextElement.h"
#include "ButtonElement.h"
#include "CanvasElement.h"

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

        Matrix & modelMatrix = object->GetModel();
        currentShader->TransferToShader("u_model", modelMatrix);

        Matrix viewMatrix = mainCamera.GetViewMatrix();

        Matrix mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;
        currentShader->TransferToShader("u_mvp", mvpMatrix);

        Archetype archetype = object->GetArchetype() & (RenderingAttributes::ATTRIB_MAX - 1);

        archetypeFunc[archetype](object);

        object->Update();

    }

    void RenderGUI(GUIElement * guiElement){

        if(guiElement == nullptr || guiElement->isActive == false)
            return;

        Matrix model = guiElement->GetModel();

        currentShader->TransferToShader("u_model", model);

        guiElement->Render();

    }

    void Execute() override{

        if(!scene)
            return;

        context->PoolEvents();
        context->SwapBuffers();
        context->CheckErrors();

        Matrix viewMatrix = mainCamera.GetViewMatrix();

        mainShader->Use();
        mainShader->TransferToShader("u_projection", projectionMatrix);
        mainShader->TransferToShader("u_view", viewMatrix);

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

        defaultMaterial = new Material(mainShader);
        defaultMaterial->mainTexture = defaultSprite;
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

                Sprite * sprite = object->GetAttribute<Sprite>();
                Mesh * mesh = object->GetAttribute<Mesh>();

                sprite->Load();
                mesh->Draw();
            };

        archetypeFunc[RenderingAttributes::LINE] =
            [](SceneObject * object){

                Sprite * sprite = object->GetAttribute<Sprite>();
                LineRenderer * renderer = object->GetAttribute<LineRenderer>();

                sprite->Load();
                renderer->Draw();

            };

    }

public:

    GraphicSystem() : System(){

        float aspect = GraphicConfig::windowWidth/(float) GraphicConfig::windowHeight;

        if(GraphicConfig::useOrthographicProjection){
            this->projectionMatrix = Matrix::Ortho(0, GraphicConfig::windowWidth, 0, GraphicConfig::windowHeight, -200.0f, 200.0f);
        }else{
            this->projectionMatrix = Matrix::Perspective(60.0f, aspect, 0.1f, 1000.0f);
        }

        this->guiProjectionMatrix = Matrix::Ortho(0, GraphicConfig::windowWidth, 0, GraphicConfig::windowHeight, -1, 10);

    }

    void OnLoad() override{

        context->Open(GraphicConfig::windowWidth, GraphicConfig::windowHeight, GraphicConfig::windowTitle);
        context->SetVSync( GraphicConfig::vsync );
        context->SetZBuffer( GraphicConfig::zbuffer );

        this->contextLogger = context->GetContextLogger();

        SetupUnhandledComponents();
        UploadMainShaders();
        SetupArchetypeFunc();

    }

    void Share(SharedNameResolver * resourceManager) override{
        this->scene = (Scene*)resourceManager->Find("scene");
        this->context = (WindowContext*)resourceManager->Find("context");
        resourceManager->Emplace("camera", &mainCamera, sizeof(Camera));
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
