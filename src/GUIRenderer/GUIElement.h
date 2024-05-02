#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "Transform.h"

class GUIElement{
public:

    bool isActive = true;
    Transform transform;

    virtual void Render() = 0;

    virtual ~GUIElement(){}

};

#endif
