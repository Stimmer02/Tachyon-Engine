#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Transform.h"
#include "GLShader.h"
#include "AttributeManager.h"
#include "Sprite.h"

#include <vector>

class SceneObject{

private:

    Entity ID;
    Archetype archetype;
    Matrix model;

    SceneObject * parent;
    std::vector<SceneObject*> childrens;

    AttributeManager * manager;
public:

    Transform transform;
    bool isActive;

    SceneObject(const Entity & ID){
        this->parent = nullptr;
        this->isActive = true;
        this->ID = ID;
        this->manager = &AttributeManager::GetInstance();
    }

    void SetParent(SceneObject * parent){
        this->parent = parent;
    }

    std::vector<SceneObject*>& GetChildrens(){
        return childrens;
    }

    void AddChildren(SceneObject * children){
        children->parent = this;
        childrens.push_back(children);
    }

    void RemoveChildren(SceneObject * children){

        children->parent = nullptr;
        std::vector<SceneObject*>::iterator it = childrens.begin();

        for(; it != childrens.end(); it++){
            if( *it == children)
                break;

        }

        if( it == childrens.end())
            return;

        childrens.erase(it);

    }

    Matrix& GetModel(){
        return model;
    }

    Entity& GetEntityID(){
        return ID;
    }

    Archetype& GetArchetype(){
        return archetype;
    }

    void Update(){

        Matrix translation = Matrix::Translate(transform.position.x, transform.position.y, transform.position.z);
        Matrix rotation = Matrix::Rotate(transform.rotation);
        Matrix scale = Matrix::Scale(transform.scale.x, transform.scale.y, transform.scale.z);

        if( parent == nullptr ){
            model = translation * rotation * scale;
        }else{
            model = parent->model * translation * rotation * scale;
        }

        for(std::vector<SceneObject*>::iterator it = childrens.begin(); it != childrens.end(); it++){
            SceneObject * children = *it;

            if(children == nullptr)
                continue;

            children->Update();
        }

    }

    template<typename T, typename... Args>
    T * AddAttribute(Args&&... args) {
        Attribute * attrib = (Attribute*)manager->AddAttribute<T>(this->ID, std::forward<Args>(args)...);
        this->archetype |= attrib->GetAttributeID();

#ifdef DEBUG

    fprintf(stdout, "[DEBUG] Current archetype %d of entity %d\n", archetype, ID);

#endif

        return (T*)attrib;
    }

    template<typename T>
    void RemoveAttribute() {
        Attribute * attrib = manager->GetAttribute<T>(this->ID);

        if( attrib == nullptr)
            return;

        manager->RemoveAttribute<T>(this->ID);
        this->archetype ^= attrib->GetAttributeID();

#ifdef DEBUG

    fprintf(stdout, "[DEBUG] Current archetype %d of entity %d\n", archetype, ID);

#endif

    }

    template<typename T>
    T * GetAttribute() {
        return manager->GetAttribute<T>(this->ID);
    }

    ~SceneObject(){

        manager->DestroyEntity(this->ID);

    }

};


#endif
