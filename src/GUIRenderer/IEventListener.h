#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

#include "EventType.h"

class IEventListener {
public:

    /// @brief Invokes concrete event on current listener
    /// @param event type of event
    virtual void OnEvent(const EventType& event) = 0;
};

#endif