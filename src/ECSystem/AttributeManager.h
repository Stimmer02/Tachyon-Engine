#ifndef ATTRIBUTEMANAGER_H
#define ATTRIBUTEMANAGER_H

#include "SceneObject.h"
#include "Attribute.h"

#include <unordered_map>
#include <typeindex>

using Archetype = unsigned char;

class AttributeManager {

private:

    AttributeManager(){

    }

public:

    void DestroyEntity(const Entity & ID){
        auto iterator = components.find(ID);

        if (iterator == components.end())
            return;

        components.erase(ID);
    }

    template<typename T, typename... Args>
    T * AddAttribute(const Entity & ID, Args&&... args){
        Attribute * attrib = (Attribute *)new T(std::forward<Args>(args)...);
        attrib->SetParent(ID);

        std::type_index index = typeid(T);

        components[ID][index] = attrib;

#ifdef DEBUG

    fprintf(stdout, "[DEBUG] New component %s attached to entity %d\n", index.name(), ID);

#endif

        return (T*)attrib;
    }

    template<typename T>
    void RemoveAttribute(const Entity & ID){
        auto iterator = components.find(ID);

        if (iterator == components.end())
            return;

        std::type_index index = typeid(T);

 #ifdef DEBUG

    fprintf(stdout, "[DEBUG] Component %s removed from entity %d\n", index.name(), ID);

#endif

        iterator->second.erase( index );
    }

    template<typename T>
    T * GetAttribute(const Entity & ID){
        auto iterator = components.find(ID);

        if (iterator == components.end())
            return nullptr;

        auto attribIterator = iterator->second;
        auto componentIterator = attribIterator.find( typeid(T) );

        if (componentIterator == attribIterator.end())
            return nullptr;

        return (T*)( componentIterator->second);
    }

    static AttributeManager& GetInstance(){
        static AttributeManager manager;
        return manager;
    }

private:
    std::unordered_map< Entity, std::unordered_map< std::type_index, Attribute* > > components;
};


#endif
