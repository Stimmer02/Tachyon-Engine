#ifndef ICLIENT_H
#define ICLIENT_H

#include "Message.h"

class IClient{
public:
    /// @brief Emplaces message with specified parameters at the end of channel's queue
    /// @param channel
    /// @param receiver
    /// @param type
    /// @param data
    virtual void SendMessage(const std::string & channel, IClient * receiver, const MessageType & type, void * data) = 0;

    /// @brief Enables handling of received messages
    /// @param message
    virtual void ReceiveMessage(const Message & message) = 0;
};

#endif
