#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vector3.h"
#include "Sprite.h"
#include "GLShader.h"

struct Material{

    const GLShader * shader;
    Sprite * mainTexture;
    Vector3 color;

    Material(const GLShader * shader){
        this->shader = shader;
    }


};

static Material * defaultMaterial;


#endif
