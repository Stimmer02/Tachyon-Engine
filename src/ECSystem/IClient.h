#ifndef ICLIENT_H
#define ICLIENT_H

#include "Message.h"

class IClient{
public:
    virtual void SendMessage(const std::string & channel, IClient * receiver, const MessageType & type, void * data) = 0;
    virtual void ReceiveMessage(const Message & message) = 0;
};

#endif