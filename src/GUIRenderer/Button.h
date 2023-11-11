#ifndef BUTTON_H
#define BUTTON_H

#include "InteractiveComponent.h"

class Button : public InteractiveComponent{
public:
    Button(const int& x, const int& y, const int& width, const int& height);
    void Render();
};


#endif