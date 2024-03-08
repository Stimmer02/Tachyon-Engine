#include "MessageBus.h"


void MessageBus::SubscribeToChannel(const std::string & _channel, IClient * _client){
    auto it = channels.find(_channel);

    Channel * channel;

    if( it == channels.end() ) {

        channel = new Channel(_channel);
        channels[ _channel ] = channel;

    }else{

        channel = channels[ _channel ];

    }

    channel->BindClient(_client);
}


void MessageBus::SendToChannel(const std::string & _channel, const Message & _message){

    auto it = channels.find( _channel );

    if( it == channels.end() ) {
        fprintf(stderr, "Channel %s does not exist.\n", _channel.c_str());
        return;
    }

    Channel * channel = it->second;

    if( channel->GetClientCount() == 0 ){
        fprintf(stderr, "Channel %s does not have any clients.\n", _channel.c_str());
        return;
    }

    channel->EmplaceMessage( _message );

}


void MessageBus::Update(){

    for(auto pair : channels){

        if(pair.second != NULL)
            pair.second->Send();

    }

}


MessageBus::~MessageBus(){

    for(auto pair : channels){

        if(pair.second != NULL)
            delete pair.second;

    }

}