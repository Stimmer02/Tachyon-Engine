#ifndef INTERACTIVEELEMENT_H
#define INTERACTIVEELEMENT_H

#include "GUIElement.h"
#include <cassert>
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

        float & xPos = transform.position.x;
        float & yPos = transform.position.y;

        return ( ((xPos - width*0.5f) < x) && ( x < (xPos + width * 0.5f)) ) &&
         ( ((yPos - height*0.5f) < y) && ( y < (yPos + height * 0.5f)) );

    }

    void Interact(){
        assert(callback && "Callback cannot be null");
        callback();
    }
};

#endif
