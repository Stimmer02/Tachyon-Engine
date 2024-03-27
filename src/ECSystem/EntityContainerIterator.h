#ifndef ENTITYCONTAINERITERATOR_H
#define ENTITYCONTAINERITERATOR_H

#include "IIterable.h"

#include <queue>
#include <stdint.h>
#include <unordered_set>

using Entity = uint32_t;

class EntityContainerIterator : public IIterable<Entity>{
private:

    std::unordered_set<Entity> * entities;
    std::queue<Entity> items;

public:
    EntityContainerIterator(std::unordered_set<Entity> * _entites){
        this->entities = _entites;
        Reset();
    }

    void Reset(){

        for( std::unordered_set<Entity>::iterator it = entities->begin(); it != entities->end(); it++){
            items.push( *it );
        }

    }

    bool HasMore(){
        return !items.empty();
    }

    Entity Next(){
        Entity front = items.front();
        items.pop();
        return front;
    }
};




#endif
