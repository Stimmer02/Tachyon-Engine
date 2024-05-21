#ifndef BOIDS_H
#define BOIDS_H

#include "Application.h"
#include <stdlib.h>
#include <ctime>

class BoidsSystem : public System {
    Scene* scene;
    Timer* timer;
    Input* input;

    std::vector<SceneObject*> boids;
    std::vector<Vector3> velocities;
    std::vector<bool> isLeader;

    const int numBoids = 300;
    const int numLeaders = 5;

    const float maxSpeed = 20.0f;
    const float maxForce = 10.0f;

    const float cellSize = 50.0f;
    std::unordered_map<int, std::unordered_map<int, std::vector<size_t>>> grid;

    std::vector<Vector3> leaderTargets;

    void UpdateGrid() {
        grid.clear();
        for (size_t i = 0; i < boids.size(); ++i) {
            Vector3& pos = boids[i]->transform.position;
            int cellX = static_cast<int>(pos.x / cellSize);
            int cellY = static_cast<int>(pos.y / cellSize);
            grid[cellX][cellY].push_back(i);
        }
    }

    std::vector<size_t> GetNearbyBoids(int cellX, int cellY) {
        std::vector<size_t> nearbyBoids;
        for (int x = cellX - 1; x <= cellX + 1; ++x) {
            for (int y = cellY - 1; y <= cellY + 1; ++y) {
                if (grid.find(x) != grid.end() && grid[x].find(y) != grid[x].end()) {
                    nearbyBoids.insert(nearbyBoids.end(), grid[x][y].begin(), grid[x][y].end());
                }
            }
        }
        return nearbyBoids;
    }

    void UpdateLeaderTargets() {
        for (size_t i = 0; i < boids.size(); ++i) {
            if (isLeader[i]) {
                Vector3& position = boids[i]->transform.position;
                Vector3& target = leaderTargets[i];

                if ((target - position).Magnitude() < 10.0f) {
                    target = Vector3(rand() % GraphicConfig::windowWidth, rand() % GraphicConfig::windowHeight, 0.0f);
                }
            }
        }
    }

    void Execute() override {
        float deltaTime = timer->GetDeltaTime() * 0.01f;

        UpdateGrid();
        UpdateLeaderTargets();

        for (size_t i = 0; i < boids.size(); ++i) {
            SceneObject* boid = boids[i];

            Vector3 force;
            if (isLeader[i]) {
                Vector3 desired = (leaderTargets[i] - boid->transform.position).Normalize() * maxSpeed;
                force = (desired - velocities[i]);

                if (force.Magnitude() > maxForce) {
                    force = force.Normalize() * maxForce;
                }

            } else {
                float minDist = INFINITY;
                Vector3 leaderPosition;
                for (size_t j = 0; j < boids.size(); ++j) {
                    if (isLeader[j]) {
                        float dist = (boids[j]->transform.position - boid->transform.position).Magnitude();
                        if (dist < minDist) {
                            minDist = dist;
                            leaderPosition = boids[j]->transform.position;
                        }
                    }
                }

                Vector3 desired = (leaderPosition - boid->transform.position).Normalize() * maxSpeed;
                force = (desired - velocities[i]);

                if (force.Magnitude() > maxForce) {
                    force = force.Normalize() * maxForce;
                }

                int cellX = static_cast<int>(boid->transform.position.x / cellSize);
                int cellY = static_cast<int>(boid->transform.position.y / cellSize);

                std::vector<size_t> nearbyBoids = GetNearbyBoids(cellX, cellY);

                for (size_t j : nearbyBoids) {
                    if (i != j) {
                        float dist = (boids[j]->transform.position - boid->transform.position).Magnitude();
                        if (dist < 10.0f) {
                            force -= (boids[j]->transform.position - boid->transform.position).Normalize() * maxForce;
                        }
                    }
                }
            }

            velocities[i] += force * deltaTime;

            if (velocities[i].Magnitude() > maxSpeed) {
                velocities[i] = velocities[i].Normalize() * maxSpeed;
            }

            boid->transform.position += velocities[i] * deltaTime;

            if (boid->transform.position.x < 0)
                boid->transform.position.x = GraphicConfig::windowWidth;
            if (boid->transform.position.y < 0)
                boid->transform.position.y = GraphicConfig::windowHeight;
            if (boid->transform.position.x > GraphicConfig::windowWidth)
                boid->transform.position.x = 0;
            if (boid->transform.position.y > GraphicConfig::windowHeight)
                boid->transform.position.y = 0;

            if (velocities[i].Magnitude() > 0)
                boid->transform.rotation = Quaternion::ToQuaternion(velocities[i].Normalize() + Vector3(0.0f, 0.0f, 90.0f));
        }
    }

public:
    BoidsSystem() {
        this->input = &Input::GetInstance();
    }

    ~BoidsSystem() {
        for (SceneObject* boid : boids) {
            scene->RemoveEntityFromScene(boid);
        }
    }

    void OnLoad() override {
        srand(static_cast<unsigned>(time(nullptr)));

        Color regular = {255, 255, 255};
        Color leader = {255, 0, 0};

        Sprite * leaderSprite = new Sprite(&leader, 1, 1);
        Sprite * regularSprite = new Sprite(&regular, 1, 1);

        Mesh* m = new Mesh();
        m->GenCone(10, 20);

        for (int i = 0; i < numBoids; ++i) {
            SceneObject* boid = scene->CreateEntity();
            scene->AddEntityToScene(boid);

            boid->transform.position = Vector3(rand() % GraphicConfig::windowWidth, rand() % GraphicConfig::windowHeight, 0.0f);

            float percent = rand() / (float)RAND_MAX;

            if (isLeader.size() < numLeaders && percent < 0.5f) {
                isLeader.push_back(true);
                leaderTargets.push_back(Vector3(rand() % GraphicConfig::windowWidth, rand() % GraphicConfig::windowHeight, 0.0f));
                boid->AddAttribute<Sprite>(leaderSprite);
            } else {
                boid->transform.scale = Vector3(0.5f, 0.5f, 1.0f);
                isLeader.push_back(false);
                leaderTargets.push_back(Vector3(0.0f, 0.0f, 0.0f));
                boid->AddAttribute<Sprite>(regularSprite);
            }

            boid->AddAttribute<Mesh>(m);

            boids.push_back(boid);
            velocities.push_back(Vector3(static_cast<float>(rand() % 100) / 100.0f - 0.5f, static_cast<float>(rand() % 100) / 100.0f - 0.5f, 0.0f));
        }
    }

    void Share(SharedNameResolver* resourceManager) override {
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
    }
};





#endif


/*

class SolarSystem : public System{

    Scene * scene;
    Camera * mainCamera;
    Timer * timer;

    SceneObject * planet;
    TextElement * text;
    Input * instance;
    Mesh * renderer;

    void Execute() override{

        static float angle;

        planet->transform.rotation = Quaternion::ToQuaternion(Vector3(angle, angle * 0.1f, angle * 0.5f));

        angle += timer->GetDeltaFrame();

        if( instance->GetKeyState(GLFW_KEY_A) == ONHOLD ){
            mainCamera->MoveBy(Vector3(-1.0,0.0,0.0), timer->GetDeltaTime());
        }

        if( instance->GetKeyState(GLFW_KEY_D) == ONHOLD ){
            mainCamera->MoveBy(Vector3(1.0,0.0,0.0), timer->GetDeltaTime());
        }

        if( instance->GetKeyState(GLFW_KEY_W) == ONHOLD ){
            mainCamera->MoveBy(Vector3(0.0,1.0,0.0), timer->GetDeltaTime());
        }

        if( instance->GetKeyState(GLFW_KEY_S) == ONHOLD ){
            mainCamera->MoveBy(Vector3(0.0,-1.0,0.0), timer->GetDeltaTime());
        }

    }

public:

    SolarSystem(){

        this->instance = &Input::GetInstance();

    }

    void OnLoad() override{

        planet = scene->CreateEntity();

        planet->transform.position.x = GraphicConfig::windowWidth * 0.5f;
        planet->transform.position.y = GraphicConfig::windowHeight * 0.5f;


        Sprite * s = planet->AddAttribute<Sprite>("resources/sprites/heart.bmp");
        s->SetTextureAttrib(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        s->SetTextureAttrib(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::vector<Vector3> vertices;

        const int numVert = 30;
        float radius = 30.0f;
        float height = 0.0f;

        for(int i = 0; i < numVert; i++){

            const float angle = i * M_PI/180.0f;

            Vector3 v = Vector3(radius * cos(angle),radius * sin(angle), height);

            vertices.push_back(v);

            radius += 0.1f;
            height += 1.0f;

        }

        renderer = planet->AddAttribute<Mesh>();
        // renderer->SetVertices(vertices.data(), vertices.size());
        renderer->GenTorus(100.0f, 50.0f);

        if( GraphicConfig::useOrthographicProjection )
            text = new TextElement("Orthographic");
        else
            text = new TextElement("Perspective");

        text->transform.position.x = 50.0f;
        text->transform.position.y = 50.0f;

        text->SetFontSize(12.0f);

        scene->AddEntityToScene(planet);
        scene->AddGUIToScene(text);

    }

    void Share(SharedNameResolver * resourceManager) override{
        this->timer = (Timer*)resourceManager->Find("timer");
        this->scene = (Scene*)resourceManager->Find("scene");
        this->mainCamera = (Camera*)resourceManager->Find("camera");
    }

};

class RubiksCubeSystem : public System {

    Scene* scene;
    Timer* timer;
    Input * instance;

    SceneObject * parent;
    std::vector<SceneObject*> cubes;

    void Execute() override {
        static float angle = 0.0f;

        Quaternion rotation = Quaternion::ToQuaternion( Vector3(angle, angle * 0.1f, angle * 0.5f) );
        parent->transform.rotation = rotation;

        for ( SceneObject * cube : cubes ){
            Quaternion rotation = Quaternion::ToQuaternion( Vector3(angle, angle * 0.1f, angle * 0.5f) );
            cube->transform.rotation = rotation;
        }

        angle += timer->GetDeltaFrame();
    }

public:
    RubiksCubeSystem() {
        this->instance = &Input::GetInstance();
    }

    ~RubiksCubeSystem(){

        scene->RemoveEntityFromScene(parent);

        for( SceneObject * obj : cubes)
            scene->RemoveEntityFromScene(obj);
    }

    void OnLoad() override {

        parent = scene->CreateEntity();
        scene->AddEntityToScene(parent);

        parent->transform.position.x = GraphicConfig::windowWidth * 0.5f;
        parent->transform.position.y = GraphicConfig::windowHeight * 0.5f;

        Color pixels;

        Sprite * s = new Sprite(&pixels, 1, 1);
        Mesh * mesh = new Mesh();
        mesh->GenCone(20.0f, 40.0f);

        int gridSize = 5;
        for (int x = 0; x < gridSize; ++x) {
            for (int y = 0; y < gridSize; ++y) {
                for (int z = 0; z < gridSize; ++z) {

                    SceneObject * cubeEntity = scene->CreateEntity();
                    scene->AddEntityToScene(cubeEntity);
                    parent->AddChildren(cubeEntity);

                    cubeEntity->transform.position = Vector3(x * 50.0f - 25, y * 50.0f - 25, z * 50.0f - 25);

                    pixels.R = (x * 312) % 255;
                    pixels.G = (y * 624 + pixels.R)%255;
                    pixels.B = (z * 379 + pixels.R * pixels. G)%255;

                    cubeEntity->AddAttribute<Mesh>(mesh);
                    cubeEntity->AddAttribute<Sprite>(&pixels, 1, 1);

                    cubes.push_back(cubeEntity);
                }
            }
        }
    }

    void Share(SharedNameResolver* resourceManager) override {
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
    }
};

*/
