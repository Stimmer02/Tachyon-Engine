#ifndef MESSAGE_H
#define MESSGAE_H

#include <string>

class IClient;

struct Message {
    std::string channel_name;
    IClient * sender;
    IClient * receiver;
    void * data;
};

#endif