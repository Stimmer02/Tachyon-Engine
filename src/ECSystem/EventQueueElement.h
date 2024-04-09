#ifndef EVENTQUEUEELEMENT_H
#define EVENTQUEUEELEMENT_H

#include <iostream>
#include <ctime>
#include "MessageType.h"

struct EventQueueElement{

    std::time_t time;
    MessageType type;
    std::string data;

    bool operator <(const EventQueueElement &other) const;
    bool operator >(const EventQueueElement &other) const;
};

#endif
