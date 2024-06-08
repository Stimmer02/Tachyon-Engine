#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "System.h"
#include "ParametrizedGLShader.h"
#include "PerlinNoise.h"

#define CHUNK_HEIGHT 100
#define CHUNK_WIDTH 100

class LandscapeSystem : public System{

private:

    Scene * scene;
    Timer * timer;

    Vector3 lightPos;
    Camera * camera;

    ParametrizedGLShader * terrainShader;
    Material * terrainMaterial;
    SceneObject * terrainObject;
    Mesh * terrain;

    float GenerateHeight(float x, float y){

        float value = 0.0;
        float totalAmplitude = 0.0;
        float localAmplitude = 100.0f;

        for (int o = 0; o < 2; ++o) {

            float perlin = Perlin::Noise(x, y);

            value +=  perlin * localAmplitude;
            totalAmplitude += localAmplitude;

            x *= 1.0f;
            y *= 1.0f;

            localAmplitude *= 1.0f;
        }

        value /= totalAmplitude;

        return value;
    }

    void GenerateTerrain(){

        terrainShader = new ParametrizedGLShader();
        terrainShader->LinkShader("resources/shaders/landscape.vert", GL_VERTEX_SHADER);
        terrainShader->LinkShader("resources/shaders/landscape.frag", GL_FRAGMENT_SHADER);
        terrainShader->Build();

        terrainMaterial = new Material(defaultMaterial);
        terrainMaterial->shader = terrainShader;

        terrainShader->AddData("lightPos", DataType::Vector, &lightPos);
        terrainShader->AddData("camPos", DataType::Vector, &camera->position);

        terrain = new Mesh();

        std::vector<Vector3> vertices;
        std::vector<unsigned int> indices;
        std::vector<float> uvs;

        for(int z = 0; z < CHUNK_HEIGHT; ++z){
            for(int x = 0; x < CHUNK_WIDTH; ++x){

                float px = x - CHUNK_WIDTH/2.0f;
                float pz = z - CHUNK_HEIGHT/2.0f;


                float u = x/(float)(CHUNK_WIDTH - 1);
                float v = z/(float)(CHUNK_HEIGHT - 1);

                float noiseVal = GenerateHeight(px * 0.5f, pz);

                Vector3 position = Vector3(px, noiseVal, pz);
                vertices.emplace_back(position);

                uvs.emplace_back(u);
                uvs.emplace_back(v);
            }
        }

        for (int z = 0; z < CHUNK_HEIGHT - 1; ++z) {
            for (int x = 0; x < CHUNK_WIDTH - 1; ++x) {
                int topLeft = z * CHUNK_WIDTH + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * CHUNK_WIDTH + x;
                int bottomRight = bottomLeft + 1;

                indices.emplace_back(topLeft);
                indices.emplace_back(bottomLeft);
                indices.emplace_back(topRight);

                indices.emplace_back(topRight);
                indices.emplace_back(bottomLeft);
                indices.emplace_back(bottomRight);
            }
        }

        terrain->SetVertices(vertices.data(), vertices.size());
        terrain->SetTexCoords(uvs.data(), uvs.size());
        terrain->SetIndices(indices.data(), indices.size());
    }

    void Execute() override{
        static float time = 0.0f;
        static float rx, ry, rz;

        // terrainObject->transform.rotation = Quaternion::ToQuaternion(Vector3(rx, ry, rz));

        // rx += time * 1e-4f * cos(time);
        // ry += time * 1e-4f * sin(time);
        // rz += time * 1e-4f;

        rx = cos(time) * cos(time * 0.5f);
        ry = sin(time * 0.5f);
        rz = sin(time) * cos(time * 0.5f);

        lightPos = Vector3(rx, ry, rz) * 200.0f;

        time += timer->GetDeltaFrame();
    }

public:

    LandscapeSystem(){

        Perlin::Initialize();

    }

    ~LandscapeSystem(){
        delete terrain;
    }

    void OnLoad() override{
        GenerateTerrain();

        lightPos = Vector3(GraphicConfig::windowWidth/2, GraphicConfig::windowHeight/3, 0.0f);

        terrainObject = scene->CreateEntity();
        terrainObject->AddAttribute<Mesh>()->GenSphere(100, 32, 32);
        terrainObject->material = terrainMaterial;
        scene->AddEntityToScene(terrainObject);

        terrainObject->transform.position = Vector3(GraphicConfig::windowWidth/2, GraphicConfig::windowHeight/3, 0.0f);
        // terrainObject->transform.scale = Vector3(10, 40, 10);

    }

    void Share(SharedNameResolver * resourceManager) override{
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
        this->camera = (Camera*)resourceManager->Find("camera");
    }

};
#endif
