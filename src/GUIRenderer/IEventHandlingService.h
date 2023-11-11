#ifndef IEVENTHANDLINGSERVICE_H
#define IEVENTHANDLINGSERVICE_H

#include "IEventListener.h"

class IEventHandlingService{
public:
    
    /// @brief Function attach listener to listeners collection
    /// @param listener
    virtual void Subscribe(const EventType & event, IEventListener * listener) = 0;

    /// @brief Function removes listener from listeners collection
    /// @param listener
    virtual void Unsubscribe(const EventType & event,IEventListener * listener) = 0;

    /// @brief Function invokes specified event on concrete listener 
    /// @param event type of event
    /// @param listener pointer to concrete listener
    virtual void Publish(const EventType & event, IEventListener * listener) = 0;

    /// @brief Function calls all assigned members of specified event
    /// @param event type of event
    virtual void Publish(const EventType & event) = 0;

};

#endif

