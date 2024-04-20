#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

using AttributeID = unsigned int;

class Attribute{
public:
    virtual ~Attribute() = default;
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
