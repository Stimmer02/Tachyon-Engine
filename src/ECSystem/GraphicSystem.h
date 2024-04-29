#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "TransformStack.h"
#include "System.h"
#include "WindowContext.h"
#include "Settings.h"
#include "GLShader.h"
#include "Scene.h"

#include "Transform.h"
#include "Sprite.h"
#include "Mesh.h"

#include "TextElement.h"

#include <functional>

using ArchetypeRenderFunc = std::function< void(Sprite *, Mesh *) >;

class GraphicSystem : public System{
private:

    Scene * scene;
    WindowContext * context;

    GLShader * mainShader;
    GLShader * guiShader;

    Sprite * defaultTexture;
    Mesh * defaultMesh;

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

        Color white[] = {255,255,255};
        this->defaultTexture = new Sprite(white, 1, 1);

        this->defaultMesh = new Mesh();

        Vector3 verts[] = {
            Vector3(-0.5f, 0.5f),
            Vector3(-0.5f, -0.5f),
            Vector3(0.5f, -0.5f),
            Vector3(0.5f, 0.5f)
        };

        float texCoord[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

        this->defaultMesh->SetVertices(verts, 4);
        this->defaultMesh->SetTexCoords(texCoord, 8);

    }

    void UploadMainShaders() {
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

    Sprite * GetDefaultTexture(){
        return defaultTexture;
    }

    Mesh * GetDefaultMesh(){
        return defaultMesh;
    }

    ~GraphicSystem(){
        delete defaultTexture;
        delete defaultMesh;
    }
};



#endif
