#include "Channel.h"


Channel::Channel(const std::string _name){
    this->name = _name;
}

void Channel::BindClient(IClient * _client){
    this->clients.emplace_back( _client );
}

void Channel::EmplaceMessage(const Message & _message){
    mutex.lock();

    this->messages.push(_message);

    mutex.unlock();
}

void Channel::Send(){

    if( messages.empty() )
        return;

    Message currentMessage = messages.front();
             
    if( currentMessage.receiver == NULL){

        for(int i = 0; i < clients.size(); ++i)
            clients[i]->ReceiveMessage(currentMessage);

    }else{

        currentMessage.receiver->ReceiveMessage(currentMessage);

    }

    messages.pop();
}

int32_t Channel::GetClientCount(){
    return clients.size();
}