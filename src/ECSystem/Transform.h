#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Attribute.h"
#include "Quaternion.h"
#include "Vector3.h"

class Transform : public AttributeType<Transform>{
public:
    Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
    // Quaternion rotation;
    Vector3 eulerAngle = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
};

#endif
