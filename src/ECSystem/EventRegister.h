#ifndef EVENTREGISTER_H
#define EVENTREGISTER_H

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <ctime>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <ctime>

#include "ILog.h"
#include "MessageType.h"
#include "EventQueueElement.h"

class EventRegister : public ILog{
private:
    std::priority_queue<EventQueueElement, std::vector<EventQueueElement>, std::greater<EventQueueElement> > eventQueue;
    std::mutex mut;
    FILE* logFile;

public:

    EventRegister();
    ~EventRegister();

    void Write(enum MessageType _type, const char * _format, ...) override;

    void Flush();

    const char* MessageTypeToString(MessageType type);

};

#endif
