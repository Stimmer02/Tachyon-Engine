#ifndef TRANSFORMATTRIBUTE_H
#define TRANSFORMATTRIBUTE_H

#include "Attribute.h"

struct Position{
    float x;
    float y;
    float z;
};

struct Quaternion{
    float x;
    float y;
    float z;
    float w;
};

class TransformAttribute : public AttributeType<TransformAttribute>{
public:
    Position position;
    Quaternion rotation;
    TransformAttribute * parent = nullptr;
};

#endif
