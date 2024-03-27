#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "IIterable.h"

#include <stdint.h>
#include <unordered_set>
#include <queue>

using Entity = uint32_t;

class EntityManager : public IIterable<Entity>{
private:

    static Entity currentEntity;

    /* BinaryTree<Entity> or unordered_set _1_placeholder; */
    std::unordered_set<Entity>::iterator currentElement;

    std::unordered_set<Entity> entites;
    std::priority_queue<Entity> freeIDs;

public:

    EntityManager(){
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

        entites.insert(entity);

        return entity;
    }

    void Destroy(const Entity & _entity){

        std::unordered_set<Entity>::iterator it = entites.find(_entity);

        if( it == entites.end() )
            return;

        freeIDs.push( _entity );

    }

    bool HasMore() {
        return currentElement != entites.end();
    }

    Entity Next() {
        return *currentElement++;
    }

};




#endif
