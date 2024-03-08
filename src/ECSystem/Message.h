#ifndef MESSAGE_H
#define MESSGAE_H

#include <string>

class IClient;

enum MessageType{
    GET,
    DROP
};


struct Message {
    IClient * sender;
    IClient * receiver;
    MessageType type;
    void * data;
};

#endif