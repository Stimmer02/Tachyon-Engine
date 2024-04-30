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
    GLuint tbo = 0;

    GLenum renderMode;

    int numIndices = 0;
    int numVertices = 0;

public:

    Mesh(const GLenum & mode = GL_TRIANGLE_FAN){
        this->renderMode = mode;
        glGenVertexArrays(1, &vao);
    }

    void SetRenderMode(const GLenum & mode){
        this->renderMode = mode;
    }

    void GenSphere(const float & radius = 1.0f, const unsigned int & numSlices = 32, const unsigned int & numStacks = 10){

        float phi, theta;
        float phiIncrement = 2.0f * M_PI / numSlices;
        float thetaIncrement = M_PI / numStacks;

        std::vector<Vector3> vertices;
        std::vector<float> uvs;
        std::vector<unsigned int> indices;

        for (int stack = 0; stack <= numStacks; ++stack) {
            theta = stack * thetaIncrement;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            for (int slice = 0; slice <= numSlices; ++slice) {
                phi = slice * phiIncrement;
                float sinPhi = std::sin(phi);
                float cosPhi = std::cos(phi);

                float x = radius * cosPhi * sinTheta;
                float y = radius * cosTheta;
                float z = radius * sinPhi * sinTheta;

                vertices.push_back( Vector3(x, y, z) );

                uvs.push_back( ((float)slice / numSlices) );
                uvs.push_back( ((float)stack / numStacks) );
            }
        }

        for (int stack = 0; stack < numStacks; ++stack) {
            for (int slice = 0; slice < numSlices; ++slice) {
                int first = (stack * (numSlices + 1)) + slice;
                int second = first + 1;
                int third = second + (numSlices + 1);
                int fourth = first + (numSlices + 1);

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(third);

                indices.push_back(first);
                indices.push_back(third);
                indices.push_back(fourth);
            }
        }


        SetVertices(vertices.data(), vertices.size());
        SetTexCoords(uvs.data(), uvs.size());
        SetIndices(indices.data(), indices.size());
        renderMode = GL_TRIANGLES;

    }

    void GenCube(const float & width, const float & height, const float & depth){


        const Vector3 vertices[] = {
            { -width * 0.5f, -height * 0.5f, -depth * 0.5f },
            { width * 0.5f, -height * 0.5f, -depth * 0.5f },
            { width * 0.5f, height * 0.5f, -depth * 0.5f },
            { -width * 0.5f, height * 0.5f, -depth * 0.5f },
            { -width * 0.5f, -height * 0.5f, depth * 0.5f },
            { width * 0.5f, -height * 0.5f, depth * 0.5f },
            { width * 0.5f, height * 0.5f, depth * 0.5f },
            { -width * 0.5f, height * 0.5f, depth * 0.5f },
        };


        const float uvs[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

        };


        const unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,

            1, 5, 6,
            1, 6, 2,

            5, 4, 7,
            5, 7, 6,

            4, 0, 3,
            4, 3, 7,

            3, 2, 6,
            3, 6, 7,

            4, 5, 1,
            4, 1, 0
        };


        SetVertices(vertices, 8);
        SetTexCoords(uvs, 48);
        SetIndices(indices, 36);
        renderMode = GL_TRIANGLES;

    }

    void SetVertices(const Vector3 * vertices, const unsigned int & numVertices){
        this->numVertices = numVertices;

        glBindVertexArray(vao);

        if( vbo == 0 ){
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertices, GL_STATIC_DRAW);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * sizeof(Vector3), vertices);
        }

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetIndices(const unsigned int * indices, const unsigned int & numIndices){
        this->numIndices = numIndices;

        glBindVertexArray(vao);

        if( ebo == 0 ){
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        }else{
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndices * sizeof(unsigned int), indices);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    AttributeID GetAttributeID() const override{
        return RenderingAttributes::MESH;
    }

    void SetTexCoords(const float* texCoords, const unsigned int & numTexCoords){
        glBindVertexArray(vao);

        if( tbo == 0 ){
            glGenBuffers(1, &tbo);
            glBindBuffer(GL_ARRAY_BUFFER, tbo);
            glBufferData(GL_ARRAY_BUFFER, numTexCoords * sizeof(float), texCoords, GL_STATIC_DRAW);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, numTexCoords * sizeof(float), texCoords);
        }

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Draw(){
        glBindVertexArray(vao);

        if( numIndices != 0 ){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glDrawElements(renderMode, numIndices, GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }else{
            glDrawArrays(renderMode, 0, numVertices);
        }

        glBindVertexArray(0);
    }

    ~Mesh(){
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

};




#endif
