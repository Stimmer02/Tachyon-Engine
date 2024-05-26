#ifndef MESH_H
#define MESH_H

#include "Attribute.h"
#include <GL/glew.h>

#include "Vector3.h"

class Mesh : public AttributeType<Mesh>{
private:

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLuint tbo = 0;
    GLuint nbo = 0;

    GLenum renderMode;

    int numIndices = 0;
    int numVertices = 0;


    void LoadFromFile(const std::string & filename){

        // TODO

    }

public:


    Mesh(const std::string & filename){
        this->renderMode = GL_TRIANGLES;
        glGenVertexArrays(1, &vao);

        LoadFromFile(filename);

    }

    Mesh(const Mesh * mesh){
        this->renderMode = mesh->renderMode;

        this->vao = mesh->vao;
        this->vbo = mesh->vbo;
        this->ebo = mesh->ebo;
        this->tbo = mesh->tbo;

        this->numIndices = mesh->numIndices;
        this->numVertices = mesh->numVertices;
    }

    Mesh(const GLenum & mode = GL_TRIANGLE_FAN){
        this->renderMode = mode;
        glGenVertexArrays(1, &vao);
    }

    void SetRenderMode(const GLenum & mode){
        this->renderMode = mode;
    }

    void GenTorus(const float & majorRadius = 1.0f, const float & minorRadius = 0.3f, const unsigned int & numMajorSegments = 32, const unsigned int & numMinorSegments = 16) {

        float phi, theta;
        float phiIncrement = 2.0f * M_PI / numMajorSegments;
        float thetaIncrement = 2.0f * M_PI / numMinorSegments;

        std::vector<Vector3> vertices;
        std::vector<Vector3> normals;
        std::vector<float> uvs;
        std::vector<unsigned int> indices;

        for (int majorSegment = 0; majorSegment <= numMajorSegments; ++majorSegment) {
            phi = majorSegment * phiIncrement;
            float cosPhi = std::cos(phi);
            float sinPhi = std::sin(phi);

            for (int minorSegment = 0; minorSegment <= numMinorSegments; ++minorSegment) {
                theta = minorSegment * thetaIncrement;
                float cosTheta = std::cos(theta);
                float sinTheta = std::sin(theta);

                float x = (majorRadius + minorRadius * cosTheta) * cosPhi;
                float y = (majorRadius + minorRadius * cosTheta) * sinPhi;
                float z = minorRadius * sinTheta;

                float nx = cosTheta * cosPhi;
                float ny = cosTheta * sinPhi;
                float nz = sinTheta;

                normals.push_back(Vector3(nx, ny, nz).Normalize());

                vertices.push_back(Vector3(x, y, z));

                uvs.push_back(((float)minorSegment / numMinorSegments));
                uvs.push_back(((float)majorSegment / numMajorSegments));
            }
        }

        for (int majorSegment = 0; majorSegment < numMajorSegments; ++majorSegment) {
            for (int minorSegment = 0; minorSegment < numMinorSegments; ++minorSegment) {
                int first = (majorSegment * (numMinorSegments + 1)) + minorSegment;
                int second = first + 1;
                int third = second + (numMinorSegments + 1);
                int fourth = first + (numMinorSegments + 1);

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(third);

                indices.push_back(first);
                indices.push_back(third);
                indices.push_back(fourth);
            }
        }

        SetVertices(vertices.data(), vertices.size());
        SetNormals(normals.data(), normals.size());
        SetTexCoords(uvs.data(), uvs.size());
        SetIndices(indices.data(), indices.size());
        renderMode = GL_TRIANGLES;
    }

    void GenCone(const float & baseRadius = 1.0f, const float & height = 2.0f, const unsigned int & numSegments = 32) {

        std::vector<Vector3> vertices;
        std::vector<float> uvs;
        std::vector<Vector3> normals;
        std::vector<unsigned int> indices;

        float angleIncrement = 2.0f * M_PI / numSegments;

        vertices.push_back(Vector3(0.0f, 0.0f, height / 2.0f));
        uvs.push_back(0.5f);
        uvs.push_back(0.5f);
        normals.push_back(Vector3(0.0f, 0.0f, 1.0f));

        for (int i = 0; i < numSegments; ++i) {
            float angle = i * angleIncrement;
            float x = baseRadius * std::cos(angle);
            float y = baseRadius * std::sin(angle);
            vertices.push_back(Vector3(x, y, -height / 2.0f));
            uvs.push_back((x + 1.0f) / 2.0f);
            uvs.push_back((y + 1.0f) / 2.0f);

            Vector3 sideVec(x, y, 0.0f);
            Vector3 heightVec(0.0f, 0.0f, height);
            Vector3 normal = Vector3::Cross(sideVec,heightVec).Normalize();
            normals.push_back(normal);
        }

        for (int i = 1; i <= numSegments; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i % numSegments + 1);
        }

        unsigned int baseCenterIndex = vertices.size();
        vertices.push_back(Vector3(0.0f, 0.0f, -height / 2.0f));
        uvs.push_back(0.5f);
        uvs.push_back(0.5f);
        normals.push_back(Vector3(0.0f, 0.0f, -1.0f));

        for (unsigned int i = 1; i <= numSegments; ++i) {
            indices.push_back(baseCenterIndex);
            indices.push_back(i % numSegments + 1);
            indices.push_back(i);
        }

        SetVertices(vertices.data(), vertices.size());
        SetTexCoords(uvs.data(), uvs.size());
        SetNormals(normals.data(), normals.size());
        SetIndices(indices.data(), indices.size());
        renderMode = GL_TRIANGLES;
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

    void GenQuad(const float & width, const float & height){

        const Vector3 verts[] = {
            {-width * 0.5f, height * 0.5f, 0.0f},
            {-width * 0.5f, -height * 0.5f, 0.0f},
            {width * 0.5f, -height * 0.5f, 0.0f},
            {width * 0.5f, height * 0.5f, 0.0f}
        };

        const float texCoord[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

        SetVertices(verts, 4);
        SetTexCoords(texCoord, 8);

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

    void SetNormals(const Vector3 * normals, const unsigned int & numNormals){

        glBindVertexArray(vao);

        if( nbo == 0 ){
            glGenBuffers(1, &nbo);
            glBindBuffer(GL_ARRAY_BUFFER, nbo);
            glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(Vector3), normals, GL_STATIC_DRAW);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, nbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, numNormals * sizeof(Vector3), normals);
        }

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

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
