#ifndef SNAKE_H
#define SNAKE_H

#include "Application.h"
#include <stdlib.h>
#include <ctime>

class SnakeSystem : public System {
    Scene * scene;
    Timer * timer;
    Input * input;

    Mesh * mesh;

    Material * fruitMaterial;
    Material * bodyMaterial, * tailMaterial;
    Material * backgroundMaterial;

    std::vector<Sprite2D*> sprites;
    std::vector<SceneObject*> snake;
    SceneObject* fruit;
    SceneObject* background;


    Vector3 direction;
    float currentRotationAngle = 0.0f;

    const float speed = 0.01f;
    const float segmentDistance = 40.0f;
    const float rotationSpeed = 2.0f;
    float timeSinceLastMove = 0.0f;

    float shakeTime = 0.0f;
    const float shakeFrequency = 1.0f;
    const float shakeAmplitude = 0.1f;

    void RotateTowards(SceneObject* segment, const Vector3& targetPosition) {
        Vector3 directionToTarget = targetPosition - segment->transform.position;
        float angle = atan2(directionToTarget.y, directionToTarget.x);
        segment->transform.rotation = Quaternion::ToQuaternion(Vector3(0.0f, 0.0f, angle - M_PI/2.0f));
    }

    void GenerateBackground(){

        sprites.push_back(new Sprite2D("resources/sprites/tiles_check.bmp"));

        backgroundMaterial = new Material(defaultMaterial);
        backgroundMaterial->mainTexture = sprites.back();

        background = scene->CreateEntity();
        scene->AddEntityToScene(background);

        background->material = backgroundMaterial;

        Mesh * mesh = background->AddAttribute<Mesh>();
        mesh->GenQuad(GraphicConfig::windowWidth, GraphicConfig::windowHeight);
        background->transform.position = Vector3(GraphicConfig::windowWidth / 2, GraphicConfig::windowHeight / 2, -0.2f);
    }

    void ShakeFruit(float deltaTime) {
        shakeTime += deltaTime;
        float shakeOffset = sin(shakeTime * shakeFrequency * 2.0f * M_PI) * shakeAmplitude;
        fruit->transform.rotation = Quaternion::ToQuaternion(Vector3(0.0f, 0.0f, shakeOffset));
    }


    void WrapAround(Vector3& position) {
        if (position.x < 0)
            position.x = GraphicConfig::windowWidth - 1;
        if (position.y < 0)
            position.y = GraphicConfig::windowHeight - 1;
        if (position.x > GraphicConfig::windowWidth)
            position.x = 0;
        if (position.y > GraphicConfig::windowHeight)
            position.y = 0;
    }

    void MoveSnake() {

        for (size_t i = snake.size() - 1; i > 0; --i){
            RotateTowards(snake[i], snake[i-1]->transform.position);
            snake[i]->transform.position = snake[i - 1]->transform.position;
            snake[i]->material = bodyMaterial;
        }

        snake.back()->material = tailMaterial;


        Vector3 newHeadPosition = snake[0]->transform.position + direction * segmentDistance;
        snake[0]->transform.position = newHeadPosition;
        RotateTowards(snake[0], newHeadPosition + direction * segmentDistance);
        WrapAround(snake[0]->transform.position);
    }

    void Grow() {
        SceneObject * newSegment = scene->CreateEntity();
        newSegment->material = bodyMaterial;
        newSegment->AddAttribute<Mesh>(mesh);
        scene->AddEntityToScene(newSegment);
        snake.push_back(newSegment);
    }

    void CheckFruitCollision() {
        if ( (snake[0]->transform.position - fruit->transform.position).Magnitude() >= segmentDistance)
            return;

        Grow();

        float angle = 2.0f * M_PI * rand()/(float)RAND_MAX;
        float radius = (GraphicConfig::windowWidth/3.0f) * rand()/(float)RAND_MAX;

        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        fruit->transform.position = Vector3(GraphicConfig::windowWidth/2 + x, GraphicConfig::windowHeight/2 + y, -0.1f);
        if(fruitMaterial != nullptr){
            int idx = rand()%2;
            fruitMaterial->mainTexture = sprites[idx];
        }

    }

    Vector3 RotateVector(const Vector3& vec, float angleDegrees) {
        constexpr float deg2rad = M_PI / 180.0f;
        float angleRadians = angleDegrees * deg2rad;
        float cosAngle = cos(angleRadians);
        float sinAngle = sin(angleRadians);
        return Vector3(
            vec.x * cosAngle - vec.y * sinAngle,
            vec.x * sinAngle + vec.y * cosAngle,
            0.0f
        );
    }

    void Execute() override {

        float deltaTime = timer->GetDeltaTime();

        if (input->GetKeyState(GLFW_KEY_LEFT) == ONHOLD) {
            currentRotationAngle += rotationSpeed;
        }
        else if (input->GetKeyState(GLFW_KEY_RIGHT) == ONHOLD) {
            currentRotationAngle -= rotationSpeed;
        }


        direction = RotateVector(Vector3(0.0f, 1.0f, 0.0f), currentRotationAngle);

        timeSinceLastMove += deltaTime;
        if (timeSinceLastMove >= 1.0f / speed) {
            timeSinceLastMove = 0.0f;
            shakeTime = 0.0f;
            MoveSnake();
            CheckFruitCollision();
            ShakeFruit(deltaTime);
        }

    }

public:
    SnakeSystem() {
        this->input = &Input::GetInstance();
        this->direction = Vector3(1.0f, 0.0f, 0.0f);
    }

    ~SnakeSystem() {
        for (SceneObject* segment : snake) {
            scene->RemoveEntityFromScene(segment);
        }

        for(Sprite2D * sprite : sprites){
            delete sprite;
        }

        if (fruit != nullptr) {
            scene->RemoveEntityFromScene(fruit);
        }

        if(fruitMaterial != nullptr){
            delete fruitMaterial;
        }

    }

    void OnLoad() override {

        sprites.push_back(new Sprite2D("resources/sprites/fruit_1.bmp"));
        sprites.back()->EnableTransparency();
        sprites.push_back(new Sprite2D("resources/sprites/fruit_2.bmp"));
        sprites.back()->EnableTransparency();

        sprites.push_back(new Sprite2D("resources/sprites/snake_head.bmp"));
        sprites.back()->EnableTransparency();
        sprites.push_back(new Sprite2D("resources/sprites/snake_body.bmp"));
        sprites.push_back(new Sprite2D("resources/sprites/snake_tail.bmp"));
        sprites.back()->EnableTransparency();

        GenerateBackground();

        mesh = new Mesh();
        mesh->GenQuad(segmentDistance, segmentDistance);

        bodyMaterial = new Material(defaultMaterial);
        bodyMaterial->mainTexture = sprites[3];

        tailMaterial = new Material(defaultMaterial);
        tailMaterial->mainTexture = sprites[4];

        SceneObject * head = scene->CreateEntity();
        head->material = new Material(defaultMaterial);
        head->material->mainTexture = sprites[2];

        scene->AddEntityToScene(head);
        head->transform.position = Vector3(GraphicConfig::windowWidth / 2, GraphicConfig::windowHeight / 2, 0.0f);
        head->AddAttribute<Mesh>(mesh);
        snake.push_back(head);

        for (int i = 1; i < 5; ++i) {
            SceneObject * segment = scene->CreateEntity();
            segment->material = bodyMaterial;
            scene->AddEntityToScene(segment);
            segment->transform.position = Vector3(GraphicConfig::windowWidth / 2 - i * segmentDistance, GraphicConfig::windowHeight / 2, 0.0f);
            segment->AddAttribute<Mesh>(mesh);
            snake.push_back(segment);
        }

        fruitMaterial = new Material(defaultMaterial);
        fruitMaterial->mainTexture = sprites[0];

        fruit = scene->CreateEntity();
        fruit->material = fruitMaterial;
        scene->AddEntityToScene(fruit);
        fruit->transform.position = Vector3(rand() % GraphicConfig::windowWidth, rand() % GraphicConfig::windowHeight, 0.0f);
        fruit->AddAttribute<Mesh>(mesh);

    }

    void Share(SharedNameResolver* resourceManager) override {
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
    }
};

#endif
