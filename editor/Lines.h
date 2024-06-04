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
    Material * mat;
    MultiSprite * multiSprite;

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

    void CreateShader(){

        shader = new GLShader();
        shader->LinkShader("resources/shaders/material.vert", GL_VERTEX_SHADER);
        shader->LinkShader("resources/shaders/material.frag", GL_FRAGMENT_SHADER);
        shader->Build();

        Image img = BitmapReader::ReadFile("resources/materials/marble/albedo.bmp");
        Image displ = BitmapReader::ReadFile("resources/materials/marble/displacement.bmp");
        Image norm = BitmapReader::ReadFile("resources/materials/marble/normal.bmp");
        Image ao = BitmapReader::ReadFile("resources/materials/marble/ao.bmp");
        Image rough = BitmapReader::ReadFile("resources/materials/marble/roughness.bmp");

        multiSprite = new MultiSprite();
        multiSprite->SetTexture("albedo", img.pixels, img.width, img.height);
        multiSprite->SetTexture("displacementMap", displ.pixels, displ.width, displ.height);
        multiSprite->SetTexture("normalMap", norm.pixels, norm.width, norm.height);
        multiSprite->SetTexture("aoMap", ao.pixels, ao.width, ao.height);
        multiSprite->SetTexture("roughnessMap", rough.pixels, rough.width, rough.height);

        multiSprite->SetTextureAttrib(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        multiSprite->SetTextureAttrib(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        delete[] img.pixels;
        delete[] norm.pixels;
        delete[] displ.pixels;
        delete[] ao.pixels;
        delete[] rough.pixels;
    }

public:

    LinesSystem(){

    }

    void OnLoad() override{

        object = scene->CreateEntity();
        scene->AddEntityToScene(object);

        // mat = new Material(defaultMaterial);
        // mat->color = Vector3(1.0f, 1.0f, 1.0f);
        // mat->mainTexture = multiSprite;
        // mat->shader = shader;

        // object->material = mat;

        renderer = object->AddAttribute<Mesh>("./resources/meshes/bunny.obj");

        object->transform.position = Vector3(GraphicConfig::windowWidth, GraphicConfig::windowHeight) * 0.5f;
    }

    void Share(SharedNameResolver * resourceManager) override{
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
    }

};


#endif
