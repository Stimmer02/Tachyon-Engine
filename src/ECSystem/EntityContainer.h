#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "EntityContainerIterator.h"

#include <stdint.h>
#include <unordered_set>
#include <vector>
#include <queue>

using Entity = uint32_t;

static Entity currentEntity;

class EntityContainer{
private:

    // TODO : Consider using BinaryTree<Entity> or skip list for better complexity

    std::unordered_set<Entity> entities;
    std::priority_queue< Entity, std::vector<Entity>, std::greater<Entity> > freeIDs;

public:

    EntityContainer(){
        currentEntity = 0;
    }

    Entity Create(){
        Entity entity;

        if( !freeIDs.empty() ){

            entity = freeIDs.top();
            freeIDs.pop();

        }else{

            entity = currentEntity++;

        }

        entities.insert(entity);

        return entity;
    }

    void Destroy(const Entity & _entity){

        std::unordered_set<Entity>::iterator it = entities.find(_entity);

        if( it == entities.end() )
            return;

        freeIDs.push( _entity );

    }

    EntityContainerIterator GetIterator(){
        return EntityContainerIterator(&entities);
    }

};




#endif
