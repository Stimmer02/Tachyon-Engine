#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Attribute.h"
#include "Quaternion.h"
#include "Vector3.h"

class Transform : public AttributeType<Transform>{
public:
    Vector3 position;
    Quaternion rotation;
};

#endif
