#ifndef EVENTREGISTER_H
#define EVENTREGISTER_H

#include <fstream>

#include "ILog.h"

class EventRegister : public ILog{
private:
    std::fstream logFile;
public:

    EventRegister();
    ~EventRegister();

    void Write(enum MessageType _type, const char * _format, ...) override;

    void Flush();

};

#endif
