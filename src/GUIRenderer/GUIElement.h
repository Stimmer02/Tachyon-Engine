#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "Transform.h"
#include "Matrix.h"

class GUIElement{

protected:

    Matrix model;

public:

    bool isActive = true;
    Transform transform;

    virtual void Render() = 0;

    Matrix& GetModel() {
        Matrix translation = Matrix::Translate(transform.position.x, transform.position.y, transform.position.z);
        Matrix rotation = Matrix::Rotate(transform.rotation);
        Matrix scale = Matrix::Scale(transform.scale.x, transform.scale.y, transform.scale.z);

        model = translation * rotation * scale;

        return model;
    }

    virtual ~GUIElement(){}

};

#endif
