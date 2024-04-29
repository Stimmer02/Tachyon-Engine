#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "Transform.h"

class GUIElement{

private:
    // placeholder _1
public:

    bool isActive = true;
    Transform transform;

    virtual void Render() = 0;

    virtual ~GUIElement(){}

};

#endif
