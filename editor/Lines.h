#ifndef LINES_H
#define LINES_H

#include "Application.h"

class LinesSystem : public System{

private:

    Scene * scene;
    Timer * timer;
    LineRenderer * renderer;

    void Execute() override{
        static float time = 0.0f;

        if(time >= 1.0f)
            renderer->SetLineWidth(50.0f * cos(time) + 51.0f, 3);

        time += timer->GetDeltaFrame();
    }

public:

    LinesSystem(){

    }

    void OnLoad() override{
        SceneObject * object = scene->CreateEntity();
        scene->AddEntityToScene(object);
        renderer = object->AddAttribute<LineRenderer>();

        object->transform.position = Vector3(GraphicConfig::windowWidth, GraphicConfig::windowHeight) * 0.5f;

        std::vector<Vector3> vertices;
        const int numVertices = 32;
        for ( int i =0; i <= numVertices; i++){
            float angle = (i/(float)numVertices) * 2.0f * M_PI;
            float x = 100.0f * cos(angle);
            float y = 100.0f * sin(angle);
            vertices.push_back( Vector3(x, y, 0.0f) );
        }

        renderer->SetVertices(vertices.data(), vertices.size());
    }

    void Share(SharedNameResolver * resourceManager) override{
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
    }

};


#endif
