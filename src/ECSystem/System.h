#ifndef SYSTEM_H
#define SYSTEM_H

// TODO : replace placeholders and Share method

#include "MessageBus.h"
#include "IClient.h"
#include "IRunnable.h"
#include "IShareable.h"

#include <atomic>

class System : public IClient, public IRunnable, public IShareable{
private:

    std::atomic<bool> isRunning;
    std::mutex mutex;
    std::queue<Message> messages;

    MessageBus * messageBus;

    /// @brief This method is called only if system receive any message
    /// @param message
    void ReceiveMessage(const Message & _message);

protected:

    // NameResolver * _1_placeholder;
    // ILogger * _2_placeholder;

    /// @brief This method is called on system startup and by default does nothing
    virtual void OnLoad();

    /// @brief This abstract method is called every iteration of system loop and must be defined by specified system
    virtual void Execute() = 0;

    /// @brief This method is called on system shutdown and by default does nothing
    virtual void OnUnload();

    /// @brief This method is called when system have any pending messages stored in internal queue and by default does nothing
    virtual void HandleMessage(const Message & message);

    /// @brief This method is called once after registration system within microkernel scope it is used to share internal resources with rest of system
    virtual void Share() = 0;

    /// @brief This method is used to send message to specified channel within message bus scope
    /// @param channel name of channel
    /// @param receiver pointer to receiver
    /// @param type type of messsage
    /// @param data pointer to data
    void SendMessage(const std::string & channel, IClient * receiver, const MessageType & type, void * data);

public:

    System();

    /// @brief Executes system loop
    void Run();

    /// @brief Sends stop signal to system thread
    void Stop();

    virtual ~System();

};


#endif
