#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include "Channel.h"

#include <map>

class MessageBus {
private:

    std::map< std::string, Channel * > channels;

public:

    /// @brief Binds client to specified communication channel
    /// @param _channel
    /// @param _client
    void SubscribeToChannel(const std::string & _channel, IClient * _client);

    /// @brief Sends message to all users in specified channel
    /// @param _channel
    /// @param _message
    void SendToChannel(const std::string & _channel, const Message & _message);

    /// @brief Updates all channels and removes top messages from queue
    void Update();

    /// @brief Returns an instance of message bus
    /// @return
    static MessageBus& GetInstance();

    /// @brief Clears data
    ~MessageBus();

};



#endif
