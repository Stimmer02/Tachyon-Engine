#ifndef LINES_H
#define LINES_H

#include "Application.h"

class LinesSystem : public System{

private:

    Scene * scene;
    SceneObject * object;
    Timer * timer;
    Mesh * renderer;

    GLShader * shader;

    void Execute() override{
        static float time = 0.0f;
        static float rx, ry, rz;

        object->transform.rotation = Quaternion::ToQuaternion(Vector3(rx, ry, rz));
        object->transform.scale = Vector3(10, 10, 10);
        rx += time * 1e-4f * cos(time);
        ry += time * 1e-4f * sin(time);
        rz += time * 1e-4f;

        time += timer->GetDeltaFrame();
    }

public:

    LinesSystem(){

    }

    void OnLoad() override{

        object = scene->CreateEntity();
        scene->AddEntityToScene(object);

        renderer = object->AddAttribute<Mesh>("./resources/meshes/dragon.obj");

        object->transform.position = Vector3(GraphicConfig::windowWidth, GraphicConfig::windowHeight) * 0.5f;
    }

    void Share(SharedNameResolver * resourceManager) override{
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
    }

};


#endif
