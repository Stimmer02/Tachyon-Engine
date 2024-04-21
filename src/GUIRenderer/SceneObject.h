#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Transform.h"
#include "MatrixUtils.h"
#include "GLShader.h"
#include "AttributeManager.h"
#include "Sprite.h"

#include <vector>

struct Quad{
    GLuint vao;
    GLuint vbo;
    GLuint tbo;
    GLuint cbo;
};

class SceneObject{

private:

    Entity ID;
    Matrix model;

    SceneObject * parent;
    std::vector<SceneObject*> childrens;

    AttributeManager * manager;

    bool isActive;

    Quad q;

public:

    Transform transform;

    SceneObject(const Entity & ID){
        this->parent = nullptr;
        this->isActive = true;
        this->ID = ID;
        this->manager = &AttributeManager::GetInstance();

        const Vector3 vert[] = {
            Vector3(-0.5f, -0.5f),
            Vector3(0.5f, -0.5f),
            Vector3(0.5f, 0.5f),
            Vector3(-0.5f, 0.5f)
        };

        const float uvs[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };

        const float col[] = {
            1.0f, 0, 0, 1.0f,
            0, 1.0f, 0, 1.0f,
            0, 0, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &q.vao);
        glBindVertexArray(q.vao);

// vertices

        glGenBuffers(1, &q.vbo);

        glBindBuffer(GL_ARRAY_BUFFER, q.vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector3), vert, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, q.vbo);

// uvs

        glGenBuffers(1, &q.tbo);

        glBindBuffer(GL_ARRAY_BUFFER, q.tbo);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), uvs, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, q.tbo);

// color

        glGenBuffers(1, &q.cbo);

        glBindBuffer(GL_ARRAY_BUFFER, q.cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STATIC_DRAW);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, q.cbo);

        glBindVertexArray(0);
    }

    void SetActivity(const bool & state){
        this->isActive = state;
    }

    bool GetActivity() const {
        return isActive;
    }

    void SetParent(SceneObject * parent){
        this->parent = parent;
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

    Matrix GetModel(){
        return model;
    }

    Entity& GetEntityID(){
        return ID;
    }

    void Update(){

        Matrix translation = MatrixUtils::Translate(transform.position.x, transform.position.y, transform.position.z);
        Matrix scale = MatrixUtils::Scale(transform.scale.x, transform.scale.y, transform.scale.z);

        if( parent == nullptr ){
            model = scale * translation;
        }else{
            model = parent->model * scale * translation;
        }

        for(std::vector<SceneObject*>::iterator it = childrens.begin(); it != childrens.end(); it++){
            SceneObject * children = *it;

            if(children == nullptr)
                continue;

            children->Update();
        }

    }

    void Render(){

        if( isActive == false )
            return;

        Matrix localModel = GetModel();

        GLShader * current = currentShader;
        current->TransferToShader("u_model", localModel);

        Sprite * sprite = manager->GetAttribute<Sprite>(ID);

        if( sprite == nullptr )
            return;

        sprite->Load();

        glBindVertexArray(q.vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        sprite->UnLoad();


        for(std::vector<SceneObject*>::iterator it = childrens.begin(); it != childrens.end(); it++){
            SceneObject * children = *it;

            if(children == nullptr)
                continue;

            children->Render();
        }

    }

    template<typename T, typename... Args>
    T * AddAttribute(Args&&... args) {
        return manager->AddAttribute<T>(this->ID, std::forward<Args>(args)...);
    }

    template<typename T>
    void RemoveAttribute() {
        manager->RemoveAttribute<T>(this->ID);
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
