#ifndef INTERACTIVECOMPONENT_H
#define INTERACTIVECOMPONENT_H

#include <functional>
#include <map>
#include <stdio.h>
#include "IEventListener.h"

using Function = std::function<void()>;

class InteractiveComponent : public IEventListener{
private:
    std::map<EventType, Function> events;
public:

    InteractiveComponent(const int& x, const int& y, const int& width, const int& height);
    void AssignEvent(const EventType& event, Function delegate);
    void OnEvent(const EventType& event);
    virtual void Render();
};


#endif