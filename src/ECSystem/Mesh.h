#ifndef MESH_H
#define MESH_H

#include "Attribute.h"
#include "Vector3.h"
#include <GL/glew.h>

class Mesh : public AttributeType<Mesh>{
private:

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    int numIndices = 0;

public:

    Mesh(){
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    void Upload(const Vector3 * vertices, const unsigned int & numVertices, const unsigned int * indices, const unsigned int & numIndices, bool realloc = true){

        this->numIndices = numIndices;

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        if(realloc){
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertices, GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        }else{
            glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * sizeof(Vector3), vertices);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndices * sizeof(unsigned int), indices);
        }

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    void Draw(GLenum mode = GL_TRIANGLES){
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(mode, numIndices, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Mesh(){
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

};




#endif
