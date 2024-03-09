#ifndef SYSTEM_H
#define SYSTEM_H

#include "MessageBus.h"
#include "IClient.h"
#include "IRunnable.h"

class System : public IClient, public IRunnable{
private:

    bool isRunning;

protected:

    MessageBus * messageBus;
    // NameResolver * _1_placeholder;
    // ILogger * _2_placeholder;

    /// @brief This function is called on system startup and by default does nothing
    virtual void OnLoad();

    virtual void Execute() = 0;

    /// @brief This function is called on system shutdown and by default does nothing
    virtual void OnUnload();

    void SendMessage(const std::string & channel, IClient * receiver, const MessageType & type, void * data);

    virtual void ReceiveMessage(const Message & message) = 0;

public:

    System(MessageBus * _messageBus);

    /// @brief Executes system loop
    void Run();

    /// @brief Sends stop signal to system thread
    void Stop();

};


#endif