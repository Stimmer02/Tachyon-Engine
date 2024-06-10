#ifndef WATER_H
#define WATER_H

#include "System.h"
#include "ParametrizedGLShader.h"
#include "PerlinNoise.h"

#define CHUNK_HEIGHT 1024
#define CHUNK_WIDTH 1024

class WaterSystem : public System{

private:

    Scene * scene;
    Timer * timer;
    Input * input;

    float time;

    Vector3 lightPos;
    Camera * camera;

    MultiSprite * multiSprite;

    ParametrizedGLShader * waterShader, * glassShader;
    Material * waterMaterial;
    SceneObject * waterObject, * pirateObject, * parentObject;
    Mesh * water;


    void Generatewater(){

        Image img = BitmapReader::ReadFile("resources/materials/water/albedo.bmp");
        Image displ = BitmapReader::ReadFile("resources/materials/water/displacement.bmp");
        Image norm = BitmapReader::ReadFile("resources/materials/water/normal.bmp");
        Image ao = BitmapReader::ReadFile("resources/materials/water/ao.bmp");
        Image specular = BitmapReader::ReadFile("resources/materials/water/specular.bmp");

        multiSprite = new MultiSprite();
        multiSprite->SetTexture("albedo", img.pixels, img.width, img.height);
        multiSprite->SetTexture("displacementMap", displ.pixels, displ.width, displ.height);
        multiSprite->SetTexture("normalMap", norm.pixels, norm.width, norm.height);
        multiSprite->SetTexture("aoMap", ao.pixels, ao.width, ao.height);
        multiSprite->SetTexture("specularMap", specular.pixels, specular.width, specular.height);

        multiSprite->SetTextureAttrib(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        multiSprite->SetTextureAttrib(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        delete[] img.pixels;
        delete[] norm.pixels;
        delete[] displ.pixels;
        delete[] ao.pixels;
        delete[] specular.pixels;

        waterMaterial = new Material(defaultMaterial);
        waterMaterial->shader = waterShader;
        waterMaterial->mainTexture = multiSprite;


        water = new Mesh();
        water->SetRenderMode(GL_TRIANGLES);

        std::vector<unsigned int> indices;
        std::vector<Vector3> vertices;
        std::vector<float> uvs;
        std::vector<Vector3> normals(CHUNK_WIDTH * CHUNK_HEIGHT, Vector3(0, 0, 0));

        for(int z = 0; z < CHUNK_HEIGHT; ++z){
            for(int x = 0; x < CHUNK_WIDTH; ++x){

                float px = (x - CHUNK_WIDTH/2.0f)/(CHUNK_WIDTH/2.0f);
                float pz = (z - CHUNK_HEIGHT/2.0f)/(CHUNK_HEIGHT/2.0f);

                float u = x/(float)(CHUNK_WIDTH - 1);
                float v = z/(float)(CHUNK_HEIGHT - 1);

                Vector3 position = Vector3(px, 0.0f, pz);
                vertices.push_back(position);

                uvs.emplace_back(u);
                uvs.emplace_back(v);
            }
        }

        for (int z = 0; z < CHUNK_HEIGHT-1; ++z) {
            for (int x = 0; x < CHUNK_WIDTH-1; ++x) {
                int topLeft = z * CHUNK_WIDTH + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * CHUNK_WIDTH + x;
                int bottomRight = bottomLeft + 1;

                indices.emplace_back(topLeft);
                indices.emplace_back(topRight);
                indices.emplace_back(bottomLeft);

                indices.emplace_back(topRight);
                indices.emplace_back(bottomLeft);
                indices.emplace_back(bottomRight);

                Vector3 e1 = vertices[bottomLeft] - vertices[topLeft];
                Vector3 e2 = vertices[topRight] - vertices[topLeft];
                Vector3 e3 = vertices[bottomRight] - vertices[topRight];

                Vector3 normal1 = Vector3::Cross(e1, e2).Normalize();
                Vector3 normal2 = Vector3::Cross(e3, e1).Normalize();

                normals[topLeft] += normal1;
                normals[bottomLeft] += normal1 + normal2;
                normals[topRight] += normal1 + normal2;
                normals[bottomRight] += normal2;

            }
        }

        for (auto& normal : normals)
            normal = normal.Normalize();

        water->SetVertices(vertices.data(), vertices.size());
        water->SetTexCoords(uvs.data(), uvs.size());
        water->SetIndices(indices.data(), indices.size());
        water->SetNormals(normals.data(), normals.size());
    }

    float WaterDistortion(float time) {
        float waveStrength = 0.0004;
        float frequency = 10.0;
        float speed = 1.0;
        float value;
        value += sin(frequency + time * speed);
        value += cos(frequency + time * speed);
        return value * waveStrength;
    }

    void Execute() override{
        static Vector3 lastPos;

        Vector3 direction = Vector3(0.0f, 0.0f, 0.0f);
        Vector3 lookDir = Vector3(0.0f, 0.0f, 0.0f);

        if( input->GetKeyState(GLFW_KEY_W) == ONHOLD){
            direction = Vector3(0.0f, 0.0f, -1.0f);
        }
        else if( input->GetKeyState(GLFW_KEY_S) == ONHOLD){
            direction = Vector3(0.0f, 0.0f, 1.0f);
        }
        else if( input->GetKeyState(GLFW_KEY_A) == ONHOLD){
            direction = Vector3(-1.0f, 0.0f, 0.0f);
        }
        else if( input->GetKeyState(GLFW_KEY_D) == ONHOLD){
            direction = Vector3(1.0f, 0.0f, 0.0f);
        }
        else if( input->GetKeyState(GLFW_KEY_SPACE) == ONHOLD){
            direction = Vector3(0.0f, 1.0f, 0.0f);
        }
        else if( input->GetKeyState(GLFW_KEY_LEFT_SHIFT) == ONHOLD){
            direction = Vector3(0.0f, -1.0f, 0.0f);
        }


        pirateObject->transform.position.y += WaterDistortion(time);


        Vector3 currentPos = input->GetMousePosition();
        lookDir = (lastPos- currentPos).Normalize() * 40.0f;
        lastPos = currentPos;

        camera->MoveBy(direction, timer->GetDeltaFrame());
        camera->RotateBy(lookDir.x, lookDir.y);

        time += timer->GetDeltaFrame() * 0.5f;
    }

public:

    WaterSystem(){

        Perlin::Initialize();

    }

    ~WaterSystem(){
        delete water;
    }

    void OnLoad() override{

        waterShader = new ParametrizedGLShader();
        waterShader->LinkShader("resources/shaders/landscape.vert", GL_VERTEX_SHADER);
        waterShader->LinkShader("resources/shaders/landscape.frag", GL_FRAGMENT_SHADER);
        waterShader->Build();

        glassShader = new ParametrizedGLShader();
        glassShader->LinkShader("resources/shaders/phong.vert", GL_VERTEX_SHADER);
        glassShader->LinkShader("resources/shaders/phong.frag", GL_FRAGMENT_SHADER);
        glassShader->Build();

        waterShader->AddData("lightPos", DataType::Vector, &lightPos);
        waterShader->AddData("time", DataType::Float, &time);
        waterShader->AddData("camPos", DataType::Vector, &camera->position);


        glassShader->AddData("lightPos", DataType::Vector, &lightPos);
        glassShader->AddData("camPos", DataType::Vector, &camera->position);

        Generatewater();

        input = &Input::GetInstance();

        parentObject = scene->CreateEntity();
        scene->AddEntityToScene(parentObject);

        waterObject = scene->CreateEntity();
        waterObject->AddAttribute<Mesh>(water);
        waterObject->material = waterMaterial;
        scene->AddEntityToScene(waterObject);


        pirateObject = scene->CreateEntity();
        pirateObject->AddAttribute<Mesh>("resources/meshes/duck.obj");
        pirateObject->material = new Material(glassShader);
        pirateObject->material->mainTexture = defaultSprite;
        pirateObject->material->color = Vector3(1.0f, 1.0f, 0.0f);
        scene->AddEntityToScene(pirateObject);

        waterObject->transform.scale = Vector3(600, 600, 600);

        parentObject->AddChildren(pirateObject);
        parentObject->AddChildren(waterObject);

        lightPos = Vector3(-4.5f, 10.0f, 8.0f);


    }

    void Share(SharedNameResolver * resourceManager) override{
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
        this->camera = (Camera*)resourceManager->Find("camera");
    }

};
#endif
