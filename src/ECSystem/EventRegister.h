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
#include "EventQueueElement.h"

class EventRegister : public ILog{
private:

    std::priority_queue<EventQueueElement, std::vector<EventQueueElement>, std::greater<EventQueueElement> > eventQueue;
    std::mutex mut;

    FILE * logFile;
    bool isSystemStream;

    const char* types[TYPE_COUNT] = TYPE_STRINGS;
public:
    EventRegister(const char* filepath);

    EventRegister(FILE * file, bool isSystemStream = false);

    ~EventRegister();

    void Write(enum LogMessageType _type, const char * _format, ...) override;

    void Flush() override;


};

#endif
