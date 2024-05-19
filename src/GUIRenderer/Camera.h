#ifndef CAMERA_H
#define CAMERA_H

#include "Settings.h"
#include "Matrix.h"

class Camera{

public:
    Vector3 position;
    Vector3 worldUp;

    Vector3 right;
    Vector3 up;
    Vector3 front;

    float yaw;
    float pitch;
    float roll;

    float fov = 45.0f;
    float movementSpeed = 1.0f;
    float sensitivity = 0.1f;

private:

    Matrix projection;

    void UpdateVectors(){

        constexpr float deg2rad = M_PI/180.0f;

        float yawInRad = yaw * deg2rad;
        float pitchInRad = pitch * deg2rad;

        this->front.x = std::cos( yawInRad ) * cos( pitchInRad );
        this->front.y = std::sin( pitchInRad );
        this->front.z = std::sin( yawInRad ) * cos( pitchInRad );

        this->front = front.Normalize();
        this->right = Vector3::Cross(worldUp, front).Normalize();
        this->up = Vector3::Cross(front, right).Normalize();
    }

public:


    Camera(){
        this->position = Vector3(0.0f, 0.0f, 0.0f);
        this->worldUp = Vector3(0.0f, 1.0f, 0.0f);
        this->front = Vector3(0.0f, 0.0f, -1.0f);
        this->up = worldUp;
        this->right = Vector3::Cross(worldUp, front).Normalize();
        this->yaw = -90.0f;
        this->pitch = 0.0f;
        UpdateVectors();
    }

    Matrix GetViewMatrix() const{

        Vector3 right = Vector3::Cross(up, front).Normalize();

        Matrix look = Matrix::LookAt(position, front, up, right);

        return look;
    }

    void ResetView(){

        this->up = worldUp;
        this->right = Vector3::Cross(worldUp, front);
        this->front = Vector3::Cross(worldUp, right);

        UpdateVectors();

    }

    void LookAt(const Vector3 & target = Vector3(0.0f, 0.0f, 0.0f)){

        Vector3 direction = (target - this->position).Normalize();
        Vector3 newRight = Vector3::Cross(worldUp, direction).Normalize();
        Vector3 newUp = Vector3::Cross(direction, newRight).Normalize();

        this->front = direction;
        this->right = newRight;
        this->up = newUp;

        UpdateVectors();

    }

    void RotateBy(const float & xOffset, const float & yOffset){

        float scaledXOffset = xOffset * sensitivity;
        float scaledYOffset = yOffset * sensitivity;

        this->yaw += scaledXOffset;
        this->pitch += scaledYOffset;

        pitch = std::fmax(-89.0f , std::fmin(pitch, 89.0f));

        UpdateVectors();

    }

    void MoveBy(const Vector3 & direction, const float & deltaTime){

        this->position =  position + direction * movementSpeed * deltaTime;

        UpdateVectors();
    }

};

#endif