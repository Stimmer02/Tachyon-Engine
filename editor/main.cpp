#include "Application.h"
#include "Mesh.h"

int main(){

    srand(time(nullptr));

    GraphicConfig::vsync = true;
    GraphicConfig::zbuffer = true;
    GraphicConfig::windowHeight = 600;
    GraphicConfig::windowWidth = 800;
    GraphicConfig::windowTitle = "Application";

    Application app;

    Scene scene;
    SceneObject * parent = scene.CreateEntity();
    scene.AddEntityToScene(parent);

    parent->AddAttribute<Mesh>();
    parent->AddAttribute<Sprite>("resources/sprites/heart.bmp");

    // Execute main loop;
    app.LoadScene(scene);
    app.Loop();


    return 0;
}
