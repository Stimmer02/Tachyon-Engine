#ifndef BOIDS_H
#define BOIDS_H

#include "Application.h"
#include <stdlib.h>
#include <ctime>

class BoidsSystem : public System {
    Scene * scene;
    Timer * timer;
    Input * input;
    Sprite2D * regularSprite;

    std::vector<SceneObject*> boids;
    std::vector<Vector3> velocities;
    std::vector<bool> isLeader;

    const int numBoids = 100;
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
            SceneObject * boid = boids[i];

            Vector3 force;
            if (isLeader[i]) {
                Vector3 desired = (leaderTargets[i] - boid->transform.position).Normalize() * maxSpeed;
                force = (desired - velocities[i]);

                if (force.Magnitude() > maxForce) {
                    force = force.Normalize() * maxForce;
                }

            } else {
                float minDist = INFINITY;
                Vector3 leaderPosition = Vector3(GraphicConfig::windowWidth*0.5, GraphicConfig::windowHeight*0.5f);
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

    GLShader * shader;
    Mesh * mesh;

public:
    BoidsSystem() {
        this->input = &Input::GetInstance();
    }

    ~BoidsSystem() {
        for (SceneObject* boid : boids)
            scene->RemoveEntityFromScene(boid);
        delete regularSprite;
    }

    void OnLoad() override {
        srand(static_cast<unsigned>(time(nullptr)));

        Color regular = {255, 255, 255};
        Color leader = {255, 0, 0};

        regularSprite = new Sprite2D(&regular, 1, 1);

        mesh = new Mesh();
        mesh->GenCone(10, 20);

        shader = new GLShader();
        shader->LinkShader("./resources/shaders/dither.vert", GL_VERTEX_SHADER);
        shader->LinkShader("./resources/shaders/dither.frag", GL_FRAGMENT_SHADER);
        shader->Build();

        Material material(shader);
        material.mainTexture = regularSprite;
        material.color = defaultMaterial->color;

        for (int i = 0; i < numBoids; ++i) {
            SceneObject* boid = scene->CreateEntity();
            scene->AddEntityToScene(boid);

            boid->material = new Material(&material);
            boid->transform.position = Vector3(rand() % GraphicConfig::windowWidth, rand() % GraphicConfig::windowHeight, 0.0f);

            float percent = rand() / (float)RAND_MAX;

            if (isLeader.size() < numLeaders && percent < 0.5f) {
                isLeader.push_back(true);
                leaderTargets.push_back(Vector3(rand() % GraphicConfig::windowWidth, rand() % GraphicConfig::windowHeight, 0.0f));
                boid->material->color = Vector3(1.0f, 0, 0);
            } else {
                boid->transform.scale = Vector3(0.5f, 0.5f, 1.0f);
                isLeader.push_back(false);
                leaderTargets.push_back(Vector3(0.0f, 0.0f, 0.0f));
            }


            boid->AddAttribute<Mesh>(mesh);

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
