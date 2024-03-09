#ifndef CHANNEL_H
#define CHANNEL_H

#include "IClient.h"

#include <stdint.h>
#include <queue>
#include <vector>
#include <mutex>

class Channel{
private:

    std::string name;
    std::queue<Message> messages;
    std::vector<IClient *> clients;
    std::mutex mutex;

public:

    /// @brief Creates channel with specified name
    /// @param _name
    Channel(const std::string _name);

    /// @brief Binds client to current channel
    /// @param _client
    void BindClient(IClient * _client);

    /// @brief Emplaces message on message queue
    /// @param _message
    void EmplaceMessage(const Message & _message);

    /// @brief Sends oldest message from queue to all or specified client
    void Send();

    /// @brief Returns number of clients present in current channel
    /// @return
    int32_t GetClientCount();

};

#endif
