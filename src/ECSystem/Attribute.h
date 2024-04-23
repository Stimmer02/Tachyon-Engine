#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

using AttributeID = unsigned int;
using Archetype = unsigned char;
using EntityID = unsigned int;

enum RenderingAttributes : AttributeID{
    NONEATTRIB = 0,
    SPRITE = 1,
    MESH = 2,
    SPRITEANDMESH = 3,
    ATTRIB_MAX = 4
};

class Attribute{
protected:

    EntityID parent = -1;

public:

    virtual AttributeID GetAttributeID() const{
        return 0;
    }

    void SetParent(const EntityID & parent){
        this->parent = parent;
    }

    EntityID& GetParent(){
        return parent;
    }


    virtual ~Attribute() = default;
    virtual void Update(){};
};

template<typename T>
AttributeID GetComponentTypeID() {
    static AttributeID typeId = 0;
    return typeId++;
}

template<typename T>
class AttributeType : public Attribute{

public:
    static AttributeID GetComponentID() {
        static AttributeID typeId = GetComponentTypeID<T>();
        return typeId;
    }
};

#endif
