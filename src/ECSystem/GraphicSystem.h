#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "System.h"
#include "WindowContext.h"
#include "GLShader.h"
#include "Scene.h"

#include "Transform.h"
#include "LineRenderer.h"
#include "Sprite2D.h"
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

    ILog * contextLogger;

    Matrix projectionMatrix;
    Matrix guiProjectionMatrix;
    Matrix viewProjectionMatrix;

    ArchetypeRenderFunc archetypeFunc[RenderingAttributes::ATTRIB_MAX];

    void RenderSceneObjects(SceneObject * object){

        if(object == nullptr || object->isActive == false)
            return;

        Matrix & modelMatrix = object->GetModel();
        Matrix viewMatrix = mainCamera.GetViewMatrix();

        Matrix mvp = viewProjectionMatrix * modelMatrix * viewMatrix;

        object->material->Use();
        currentShader->TransferToShader("u_mvp", mvp);

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

        std::vector<SceneObject *>& objects = scene->GetSceneObjects();
        std::vector<GUIElement *>& guiElements = scene->GetGUIElements();

        viewProjectionMatrix = projectionMatrix ;

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
        defaultSprite = new Sprite2D(white, 1, 1);

        defaultMaterial = new Material(mainShader);
        defaultMaterial->mainTexture = defaultSprite;
    }

    void UploadMainShaders() {

        contextLogger->Write(LogMessageType::M_INFO, "Compiling shaders\n");

        this->mainShader = new GLShader();
        this->mainShader->LinkShader("./resources/shaders/default.vert", GL_VERTEX_SHADER);
        this->mainShader->LinkShader("./resources/shaders/default.frag", GL_FRAGMENT_SHADER);
        this->mainShader->Build();

        this->guiShader = new GLShader();
        this->guiShader->LinkShader("./resources/shaders/gui.vert", GL_VERTEX_SHADER);
        this->guiShader->LinkShader("./resources/shaders/default.frag", GL_FRAGMENT_SHADER);
        this->guiShader->Build();
    }

    void SetupArchetypeFunc(){

        contextLogger->Write(LogMessageType::M_INFO, "Building rendering archetypes\n");

        archetypeFunc[RenderingAttributes::NONEATTRIB] =
            [this](SceneObject * object){
                return ;
            };

        archetypeFunc[RenderingAttributes::MESH] =
            [this](SceneObject * object){
                Mesh * mesh = object->GetAttribute<Mesh>();
                mesh->Draw();
            };

        archetypeFunc[RenderingAttributes::LINE] =
            [](SceneObject * object){
                LineRenderer * renderer = object->GetAttribute<LineRenderer>();
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

        UploadMainShaders();
        SetupUnhandledComponents();
        SetupArchetypeFunc();

    }

    void Share(SharedNameResolver * resourceManager) override{
        this->scene = (Scene*)resourceManager->Find("scene");
        this->context = (WindowContext*)resourceManager->Find("context");
        resourceManager->Emplace("camera", &mainCamera, sizeof(Camera));
    }

    ~GraphicSystem(){
        delete mainShader;
        delete guiShader;
        delete defaultSprite;
    }
};



#endif
