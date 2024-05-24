#ifndef MICROKERNEL_H
#define MICROKERNEL_H

#include "System.h"
#include "EntityContainer.h"
#include "EntityContainerIterator.h"

#include <vector>

class Microkernel {
private:

    EntityContainer entityContainer;
    std::vector< std::pair<System * , bool> > systems;

public:

    Microkernel(){

    }

    void RegisterSystem(const System * _system){
        systems.emplace_back( std::make_pair(_system, false) );
    }

    ~Microkernel(){

        for(uint32_t i = 0; i < systems.size(); i++){

            if( systems[i].second == true )
                delete systems[i].first;

        }

    }
};

#endif
