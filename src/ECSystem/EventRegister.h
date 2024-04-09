#ifndef EVENTREGISTER_H
#define EVENTREGISTER_H

#include <fstream>
#include <iostream>
#include <mutex>

#include "ILog.h"

class EventRegister : public ILog{
private:
    std::mutex mut;
    std::fstream logFile;
public:

    EventRegister();
    ~EventRegister();

    void Write(enum MessageType _type, const char * _format, ...) override;

    void Flush();

};

#endif
