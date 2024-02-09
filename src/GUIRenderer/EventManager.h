#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "IEventHandlingService.h"
#include <stdio.h>
#include <map>
#include <list>

class EventManager : public IEventHandlingService{
public:

    /// @brief Method assign provided listener to list of listeners of specified event type
    /// @param event type of event
    /// @param listener object listening to event occur
    void Subscribe(const EventType & event, const IEventListener * listener);

    /// @brief Method removes provided listener from list of listeners to specified event
    /// @param event type of event
    /// @param listener object listening to event occur
    void Unsubscribe(const EventType & event, const IEventListener * listener);

    /// @brief Method invokes event on specified listener
    /// @param event type of event
    /// @param listener specified listener target
    void Publish(const EventType & event, const IEventListener * listener);

    /// @brief Method invokes event on all listeners
    /// @param event tyoe of event
    void Publish(const EventType & event);

private:
    std::map<EventType, std::list<const IEventListener*>> listeners;
};

#endif
