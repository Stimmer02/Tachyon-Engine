#ifndef INTERACTIVEELEMENT_H
#define INTERACTIVEELEMENT_H

#include "GUIElement.h"
#include <functional>

using Callback = std::function<void()>;

class InteractiveElement : public GUIElement{

private:

    Callback callback;

public:

    int width;
    int height;

    InteractiveElement(const int & width, const int & height, const Callback & callbackFunc) : GUIElement(){
        this->callback = callbackFunc;
        this->width = width;
        this->height = height;
    }

    virtual void Render() = 0;

    virtual bool isInBound(const int & x, const int & y){
        return x >= transform.position.x && x <= transform.position.x + width &&
               y >= transform.position.y && y <= transform.position.y + height;
    }

    void Interact(){
        if( callback )
            callback();
    }
};

#endif
