#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "Transform.h"
#include "Matrix.h"

class GUIElement{
public:

    bool isActive = true;
    Transform transform;
    Matrix model;

    virtual void Render() = 0;

    virtual ~GUIElement(){}

};

#endif
