#ifndef EVENTREGISTER_H
#define EVENTREGISTER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <ctime>
#include <chrono>
#include <cstdarg>

#include "ILog.h"
#include "MessageType.h"

class EventRegister : public ILog{
private:
    std::priority_queue<std::pair<std::pair<std::time_t, MessageType>, std::string> > eventQueue;
    std::mutex mut;
    std::fstream logFile;

    bool queueElementsComperator(const std::pair<std::pair<std::time_t, MessageType>, std::string> &var1, const std::pair<std::pair<std::time_t, MessageType>, std::string>  &var2);
public:

    EventRegister();
    ~EventRegister();

    void Write(enum MessageType _type, const char * _format, ...) override;

    void Flush();

};

#endif
