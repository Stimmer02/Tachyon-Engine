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

        for (auto& componentPair : components) {

            std::unordered_map<Entity, Attribute*>& entityMap = componentPair.second;

            auto it = entityMap.find(ID);
            if (it != entityMap.end()) {
                delete it->second;
                it->second = nullptr;
                entityMap.erase(it);
            }
        }

    }

    template<typename T, typename... Args>
    T * AddAttribute(const Entity & ID, Args&&... args){
        Attribute * attrib = (Attribute *)new T(std::forward<Args>(args)...);
        attrib->SetParent(ID);

        std::type_index index = typeid(T);

        components[index][ID] = attrib;

#ifdef DEBUG

    fprintf(stdout, "[DEBUG] New attribute %s attached to entity %d\n", index.name(), ID);

#endif

        return (T*)attrib;
    }

    template<typename T>
    void RemoveAttribute(const Entity & ID){
        std::type_index index = typeid(T);

        auto iterator = components.find(index);

        if (iterator == components.end())
            return;

        std::unordered_map<Entity, Attribute *>& entityMap = iterator->second;

        auto componentIterator = entityMap.find(ID);

        if (componentIterator == entityMap.end())
            return;

        delete componentIterator->second;

#ifdef DEBUG

    fprintf(stdout, "[DEBUG] Attribute %s erased from entity %d\n", index.name(), ID);

#endif

        entityMap.erase( componentIterator );
    }

    template<typename T>
    T * GetAttribute(const Entity & ID){
        std::type_index index = typeid(T);

        auto iterator = components.find(index);

        if (iterator == components.end())
            return nullptr;

        std::unordered_map<Entity, Attribute *>& entityMap = iterator->second;

        auto componentIterator = entityMap.find(ID);

        if (componentIterator == entityMap.end())
            return nullptr;

        return (T*) componentIterator->second;

    }

    static AttributeManager& GetInstance(){
        static AttributeManager manager;
        return manager;
    }

private:
    std::unordered_map< std::type_index, std::unordered_map< Entity, Attribute* > > components;
};


#endif
