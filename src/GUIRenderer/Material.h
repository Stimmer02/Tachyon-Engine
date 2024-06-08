#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vector3.h"
#include "Sprite.h"
#include "GLShader.h"

#include <stdint.h>

class Material{

private:

    uint32_t CalculateChecksum() const {
        std::string data = std::to_string((uintptr_t)(shader));
        data += (uintptr_t)(mainTexture);
        data += std::to_string(color.x) + std::to_string(color.y) + std::to_string(color.z);

        uint32_t checksum = 0;
        for (char c : data)
            checksum += (uint32_t)(c);

        return checksum;
    }

public:

    GLShader * shader;
    Sprite * mainTexture;
    Vector3 color;

    uint32_t checksum;

    Material(const GLShader * shader){
        this->shader = (GLShader *)shader;
        this->checksum = CalculateChecksum();
        this->color = Vector3(1.0f, 1.0f, 1.0f);
    }

    Material(const Material * material){
        this->shader = material->shader;
        this->mainTexture = material->mainTexture;
        this->color = material->color;
        this->checksum = material->checksum;
    }

    virtual void Use(){
        this->shader->Use();
        this->mainTexture->Load(shader);
        this->shader->TransferToShader("u_color", color);
    }

    virtual void Dispose(){
        this->shader->Dispose();
        this->mainTexture->UnLoad();
    }

    bool operator==(const Material& other) const {
        return checksum == other.checksum;
    }

    bool operator!=(const Material& other) const {
        return !(*this == other);
    }

};

static Material * defaultMaterial = nullptr;


#endif
