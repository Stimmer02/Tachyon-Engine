#ifndef ATTRIBUTECONTAINER_H
#define ATTRIBUTECONTAINER_H

#include <stdint.h>
#include <cstring>
#include <queue>
#include <map>

template<class Component>
class AttributeContainer{
private:

    uint32_t currentID;
    std::priority_queue<uint32_t, std::vector<uint32_t>, std::greater<uint32_t>> freeIDs;

    //TODO : We should consider another method of mapping entity id to component id, map provides complexity O(log(n))

    std::map<uint32_t, uint32_t> mapper;
    Component ** components;

    float remapThreshold;

    uint32_t capacity;
    uint32_t size;

    void RemapAndResize(){
        std::map<uint32_t, uint32_t> newMapper;
        Component ** newComponents = new Component*[ size ];
        uint32_t newID = 0;

        for (const auto& entry : mapper) {

            uint32_t oldID = entry.first;
            uint32_t componentID = entry.second;

            newMapper[oldID] = newID;
            newComponents[newID] = components[componentID];

            newID++;
        }

        delete[] components;

        this->components = newComponents;
        this->mapper = newMapper;
        this->currentID = newID;
        this->freeIDs = std::priority_queue<uint32_t, std::vector<uint32_t>, std::greater<uint32_t>>();
        this->capacity = ( size > 10 ? size : 10);

    }

public:

    AttributeContainer(){
        this->size = 0;
        this->capacity = 10;
        this->currentID = 0;
        this->components = new Component*[ capacity ];
        this->remapThreshold = 0.0f;
    }

    Component * AddElement(const uint32_t & _id){

        uint32_t componentID;

        if( freeIDs.empty() == false){
            componentID = freeIDs.top();
            freeIDs.pop();
        }else{
            componentID = currentID++;
        }

        if( componentID >= capacity){

            Component ** bigger = new Component*[ capacity * 2 ];

            memcpy(bigger, components, capacity * sizeof( Component * ));

            delete[] components;
            
            capacity *= 2;
            components = bigger;

        }

        Component * component = new Component();

        mapper[ _id ] = componentID;
        components[ componentID ] = component;

        size += 1;

        return component;
    }

    Component * GetElement(const uint32_t & _id){
        auto it = mapper.find(_id);

        if( it == mapper.end())
            return nullptr;

        uint32_t componentID = it->second;

        return components[ componentID ];
    }

    void RemoveElement(const uint32_t & _id){
        auto it = mapper.find( _id );

        if( it == mapper.end() )
            return;
            

        uint32_t componentID = it->second;

        delete components[ componentID ];
        freeIDs.push( componentID );

        mapper.erase(it);
        size--;

        if( size <= (capacity * remapThreshold) )
            RemapAndResize();
    }

    uint32_t GetSize() const{
        return size;
    }

    uint32_t GetCapacity() const{
        return capacity;
    }

    void SetRemapThresholdPercentage(const float & minElements){
        this->remapThreshold = minElements;
    }

    ~AttributeContainer(){

        for(uint32_t i = 0; i < capacity; ++i){

            if( components[i] )
                delete components[i];

        }
                
        delete[] components;

    }
};



#endif