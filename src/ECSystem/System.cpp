#include "System.h"


void System::SendMessage(const std::string & channel, IClient * receiver, const MessageType & type, void * data){

    Message message;
    message.sender = this;
    message.receiver = receiver;
    message.type = type;
    message.data = data;

    messageBus->SendToChannel(channel, message);

};

System::System(){
    this->messageBus = &MessageBus::GetInstance();
    this->isRunning = false;

#ifdef DEBUG

    fprintf(stdout, "[DEBUG] Loading new system\n");

#endif

    OnLoad();

}

System::~System(){

#ifdef DEBUG

    fprintf(stdout, "[DEBUG] Disposing system\n");

#endif

    OnUnload();

}

void System::ReceiveMessage(const Message & _message){
    std::lock_guard<std::mutex> lock(mutex);
    messages.push( _message );
}

void System::OnLoad(){
    // This function does nothing by default but may be overloaded
}

void System::OnUnload(){
    // This function does nothing by default but may be overloaded
}

void System::HandleMessage(const Message & message){
    // This function does nothing by default but may be overloaded
}

void System::Stop(){
    isRunning = false;
}

void System::Run(){

    // isRunning = true;

    // while( isRunning ){

        // Its written like that on purpose for cache coherence and minimalization of cache miss

        if( !messages.empty() ){
            Message message = messages.front();
            messages.pop();
            HandleMessage(message);
            Execute();
        }else{
            Execute();
        }

    // }

}
