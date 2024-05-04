#ifndef EVENTQUEUEELEMENT_H
#define EVENTQUEUEELEMENT_H

#include <iostream>
#include <ctime>
#include <chrono>
#include "MessageType.h"

struct EventQueueElement{

    std::chrono::nanoseconds time;
    LogMessageType type;
    std::string data;

    bool operator <(const EventQueueElement &other) const;
    bool operator >(const EventQueueElement &other) const;
};

#endif
