#include "System.h"


void System::SendMessage(const std::string & channel, IClient * receiver, const MessageType & type, void * data){

    Message message;
    message.sender = this;
    message.receiver = receiver;
    message.type = type;
    message.data = data;

    messageBus->SendToChannel(channel, message);

};

System::System(MessageBus * _messageBus){
    this->messageBus = _messageBus;
    this->isRunning = false;
}

void System::OnLoad(){
    // This function does nothing but may be overloaded
}

void System::OnUnload(){
    // This function does nothing but may be overloaded
}

void System::Stop(){
    this->isRunning = false;
}

void System::Run(){

    OnLoad();

    isRunning = true;

    while( isRunning )
        Execute();

    OnUnload();

}
