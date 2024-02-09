#ifndef INTERACTIVECOMPONENT_H
#define INTERACTIVECOMPONENT_H

#include <functional>
#include <map>
#include <stdio.h>
#include "Component.h"
#include "IEventListener.h"

using Function = std::function<void()>;


class InteractiveComponent : public IEventListener, public Component{
private:
    std::map<EventType, Function> events;
public:

    InteractiveComponent(const int& x, const int& y, const int& width, const int& height);

    /// @brief Function assigns action which will happen after specific event occur
    /// @param event type of event
    /// @param delegate action represented as void (*)() or lambda
    void AssignEvent(const EventType& event, Function delegate);

    /// @brief Invokes action assigned with specified event
    /// @param event type of event
    void OnEvent(const EventType& event);

};


#endif
