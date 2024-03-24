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

    void RemapAndResize();

public:

    AttributeContainer();

    Component * AddElement(const uint32_t & _id);

    Component * GetElement(const uint32_t & _id);

    void RemoveElement(const uint32_t & _id);

    uint32_t GetSize() const;

    uint32_t GetCapacity() const;

    void SetRemapThresholdPercentage(const float & minElements);

    ~AttributeContainer();
};



#endif