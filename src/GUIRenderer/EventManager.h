#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "IEventHandlingService.h"
#include <stdio.h>
#include <map>
#include <list>

class EventManager : public IEventHandlingService{
public:

    void Subscribe(const EventType & event, IEventListener * listener);

    void Unsubscribe(const EventType & event,IEventListener * listener);

    void Publish(const EventType & event, IEventListener * listener);

    void Publish(const EventType & event);

private:
    std::map<EventType, std::list<IEventListener*>> listeners;
};


#endif