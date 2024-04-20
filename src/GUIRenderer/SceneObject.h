#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "MatrixUtils.h"
#include "Transform.h"
#include "GLShader.h"
#include "EntityContainer.h"
#include <list>

#include <GL/glew.h>

struct GraphicObject{
    GLuint vao = 0;
    GLuint vbo = 0;
};

class SceneObject{

private:

    Entity ID;

    Transform transform;
    Matrix model;

    SceneObject * parent;
    std::list<SceneObject*> childrens;

    bool isActive;

    //Temporary
    GraphicObject graphic;

public:

    SceneObject(const Entity & ID){
        this->parent = nullptr;
        this->isActive = true;
        this->ID = ID;

        const Vector3 verts[] = {
            Vector3(-0.25, -0.25),
            Vector3(0.25, -0.25),
            Vector3(0.25, 0.25),
            Vector3(-0.25, 0.25)
            };

        glGenBuffers(1, &graphic.vbo);
        glGenVertexArrays(1, &graphic.vao);

        glBindVertexArray(graphic.vao);

        glBindBuffer(GL_ARRAY_BUFFER, graphic.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
        std::list<SceneObject*>::iterator it = childrens.begin();

        for(; it != childrens.end(); it++){
            if( *it == children)
                break;

        }

        if( it == childrens.end())
            return;

        childrens.erase(it);

    }

    Matrix GetModel(){

        if( parent == nullptr)
            return model;


        Matrix modelMatrix = parent->GetModel() * model;

        return modelMatrix;
    }

    Transform& GetTransform(){
        return transform;
    }

    Entity& GetEntityID(){
        return ID;
    }

    void Update(){

        Matrix translation = MatrixUtils::Translate(transform.position.x, transform.position.y, transform.position.z);
        Matrix scale = MatrixUtils::Scale(transform.scale.x, transform.scale.y, transform.scale.z);

        model = scale * translation;

        for(std::list<SceneObject*>::iterator it = childrens.begin(); it != childrens.end(); it++){
            SceneObject * children = *it;

            if(children == nullptr)
                continue;

            children->Update();
        }

    }

    void Render(){

        if( isActive == false )
            return;

        GLShader * current = currentShader;

        Vector3 vec = transform.position.Normalize() * 0.5f + Vector3(0.5f, 0.5f, 0.5f);

        Matrix localModel = GetModel();

        current->TransferToShader("u_color", vec);
        current->TransferToShader("u_model", localModel);

        glBindVertexArray(graphic.vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        for(std::list<SceneObject*>::iterator it = childrens.begin(); it != childrens.end(); it++){
            SceneObject * children = *it;

            if(children == nullptr)
                continue;

            children->Render();
        }

    }

    ~SceneObject(){



    }

};


#endif
