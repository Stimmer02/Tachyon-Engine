#ifndef MICROKERNEL_H
#define MICROKERNEL_H

#include "System.h"
#include "MessageBus.h"

#include <vector>

class Microkernel {
private:

    MessageBus bus;
    std::vector<System *> systems;

public:
    Microkernel();

    void RegisterSystem(const System * _system){
        systems.emplace_back(_system);
    }

    MessageBus & GetMessageBusHandle(){
        return bus;
    }

    ~Microkernel();
};

#endif