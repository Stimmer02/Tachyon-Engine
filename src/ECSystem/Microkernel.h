#ifndef MICROKERNEL_H
#define MICROKERNEL_H

#include "System.h"
#include "EntityContainer.h"
#include "EntityContainerIterator.h"

#include <vector>

class Microkernel {
private:

    EntityContainer entityContainer;
    std::vector< System * > systems;

public:

    Microkernel(){

    }

    void RegisterSystem(const System * _system){
        systems.emplace_back(_system);
    }


    ~Microkernel(){

    }
};

#endif
