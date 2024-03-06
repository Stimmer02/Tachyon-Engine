#ifndef CHANNEL_H
#define CHANNEL_H

#include "IClient.h"
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

    Channel(const std::string _name);

    void BindClient(IClient * _client);

    void EmplaceMessage(const Message & _message);
    
    void Send();

};

#endif