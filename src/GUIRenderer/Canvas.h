#ifndef CANVAS_H
#define CANVAS_H

#include "InteractiveComponent.h"

class Canvas : public InteractiveComponent{
public:

    Canvas(const int& x, const int& y, const int& width, const int& height);

    void Render();
};


#endif
