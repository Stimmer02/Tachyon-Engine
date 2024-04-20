#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Matrix.h"
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

        glGenBuffers(1, &graphic.vbo);
        glGenVertexArrays(1, &graphic.vao);

        glBindVertexArray(graphic.vao);
        glBindBuffer(GL_ARRAY_BUFFER, graphic.vbo);

        const Vector3 verts[] = {
            Vector3(-10, -10),
            Vector3(10, -10),
            Vector3(10, 10),
            Vector3(-10, 10)
            };

        glBufferData(GL_ARRAY_BUFFER, sizeof(verts) * sizeof(Vector3), verts, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0 );
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
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
        childrens.push_back(children);
    }

    void RemoveChildren(SceneObject * children){

        std::list<SceneObject*>::iterator it = childrens.begin();

        for(; it != childrens.end(); it++){
            if( *it == children)
                break;

        }

        if( it == childrens.end())
            return;

        childrens.erase(it);

    }

    Matrix& GetModel(){

        if( parent == nullptr)
            return model;


        Matrix modelMatrix = parent->GetModel() * model;

        return model;
    }

    Transform& GetTransform(){
        return transform;
    }

    Entity& GetEntityID(){
        return ID;
    }

    void Update(){




    }

    void Render(){

        if( isActive == false )
            return;

        GLShader * current = currentShader;
        current->TransferToShader("position", transform.position);

        glBindVertexArray(graphic.vao);
        glDrawArrays(GL_LINES, 0, 4);
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
