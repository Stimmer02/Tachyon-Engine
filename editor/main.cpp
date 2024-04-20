#include "Application.h"

SceneObject * child, * parent, * grandchild;

class MovementSystem : public System{

    Scene * scene;

    void Execute() override{

        if(child == nullptr)
            return;

        static float angle = 0.0f;

        // Transform& ptransform = parent->GetTransform();

        // ptransform.position.y = sin(angle) * 0.5f;

        Transform& transform = child->GetTransform();

        transform.position.x = cos(angle) * 0.5f;

        Transform& gtransform = grandchild->GetTransform();

        gtransform.position.y = sin(angle) * 0.5f;

        angle *= (angle < 180.0f);
        angle += 0.1f;

    }

    void Share() override{


    }

public:

    MovementSystem(Scene * scene){
        this->scene = scene;
    }


};

int main(){

    GraphicConfig::vsync = true;
    GraphicConfig::zbuffer = true;
    GraphicConfig::windowHeight = 1000;
    GraphicConfig::windowWidth = 1000;
    GraphicConfig::windowTitle = "Application";

    Application app;

    GLShader mainShader;
    mainShader.LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
    mainShader.LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
    mainShader.Build();

    Scene scene;

    parent = scene.CreateEntity();
    child = scene.CreateEntity();
    grandchild = scene.CreateEntity();

    child->GetTransform().position = {0.5f, 0, 0.1f};
    parent->AddChildren(child);
    child->AddChildren(grandchild);

    scene.AddEntityToScene(parent);

    MovementSystem system(&scene);

    mainShader.Use();

    // Execute main loop;
    app.RegisterSystem(&system);
    app.LoadScene(scene);
    app.Loop();

    mainShader.Dispose();

    return 0;
}
