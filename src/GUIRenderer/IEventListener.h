#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

#include "EventType.h"
#include <stdint.h>

class IEventListener {
public:

    /// @brief Invokes concrete event on current listener
    /// @param event type of event
    virtual void OnEvent(const EventType& event) = 0;

    /// @brief Returns GUID of current listener
    virtual uint32_t GetGUID() = 0;

};

#endif
