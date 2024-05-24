# System Class

Represents a system component that can receive, process, and send messages within a message bus.

## Description

The `System` class is designed to be inherited by specific system implementations. It provides a framework for initializing, running, and shutting down a system, as well as handling messages and sharing resources within a microkernel scope.

## Members

- `isRunning`: Flag indicating whether the system is running.
- `mutex`: Mutex for synchronizing access to internal data structures.
- `messages`: Queue for storing incoming messages.
- `messageBus`: Pointer to the message bus used for communication.

## Methods

### `OnLoad()`

Callback method called on system startup. Override this method to perform initialization steps.

### `Execute()`

Abstract method called on each iteration of the system loop. Derived classes must implement this to define system behavior.

### `OnUnload()`

Callback method called on system shutdown. Override this to perform cleanup steps.

### `HandleMessage(const Message& message)`

Callback method called when the system has pending messages. Override to define message handling.

### `Share()`

Method called once after the system is registered within a microkernel scope. Implement to share internal resources.

### `SendMessage(const std::string& channel, IClient* receiver, const MessageType& type, void* data)`

Sends a message to a specified channel within the message bus.

### `System(MessageBus* _messageBus)`

Constructor for the System class.

### `Run()`

Executes the main loop of the system.

### `Stop()`

Sends a stop signal to the system thread.
