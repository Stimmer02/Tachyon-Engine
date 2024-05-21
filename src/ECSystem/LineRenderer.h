#ifndef LINERENDERER_H
#define LINERENDERER_H

#include "Attribute.h"
#include "Vector3.h"
#include <vector>
#include <GL/glew.h>

class LineRenderer : public AttributeType<LineRenderer>{
private:

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLuint tbo = 0;

    GLenum renderMode;

    int numVertices = 0;
    int numIndices = 0;

    float startWidth;
    float endWidth;

    std::vector<Vector3> vertices;

    void RebuildQuads(){

        if( vertices.empty() )
            return;

        std::vector<Vector3> polygon;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < vertices.size() - 2; ++i) {

            float t = i / (float)(vertices.size() - 1);
            float startLineWidth = startWidth + (endWidth - startWidth) * t;
            float endLineWidth = startWidth + (endWidth - startWidth) * (t + 1.0f / (vertices.size() - 1));

            Vector3 currentVertex = vertices[i];
            Vector3 nextVertex = vertices[i + 1];

            Vector3 direction = (nextVertex - currentVertex).Normalize();
            Vector3 perpendicular = Vector3(-direction.y, direction.x, 0.0f);

            Vector3 v0 = currentVertex - perpendicular * startLineWidth;
            Vector3 v1 = currentVertex + perpendicular * startLineWidth;
            Vector3 v2 = nextVertex - perpendicular * endLineWidth;
            Vector3 v3 = nextVertex + perpendicular * endLineWidth;

            polygon.push_back(v2);
            polygon.push_back(v3);

            if (i > 0) {
                indices.push_back(i * 2 - 1);
                indices.push_back(i * 2);
                indices.push_back(i * 2 + 1);
            }

            indices.push_back(i * 2);
            indices.push_back(i * 2 + 2);
            indices.push_back(i * 2 + 1);
        }

        indices.push_back((vertices.size() - 2) * 2 + 1);
        indices.push_back((vertices.size() - 2) * 2 + 2);
        indices.push_back((vertices.size() - 2) * 2 + 3);

        glBindVertexArray(vao);

        if( vbo != 0 ){
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }

        this->numVertices = polygon.size();

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, polygon.size() * sizeof(Vector3), polygon.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        if( ebo != 0 ){
            glDeleteBuffers(1, &ebo);
            ebo = 0;
        }

        this->numIndices = indices.size();

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void RebuildTextureCoords(){

        if( vertices.empty() )
            return;

        std::vector<float> textureCoords;

        float totalLength = 0.0f;
        float textureCoordScale = 0.0f;
        float textureCoordOffset = 0.0f;

        for (unsigned int i = 0; i < vertices.size() - 1; ++i)
            totalLength += (vertices[i + 1] - vertices[i]).Magnitude();


        if (totalLength > 1e-6f)
            textureCoordScale = 1.0f / totalLength;

        for(int i = 0; i < vertices.size()-2; i++){

            float t = i / (float)(vertices.size() - 1);

            textureCoords.push_back(t);
            textureCoords.push_back(0.0f);
            textureCoords.push_back(t);
            textureCoords.push_back(1.0f);

        }

        if( tbo != 0 ){
            glDeleteBuffers(1, &tbo);
            tbo = 0;
        }

        glBindVertexArray(vao);

        glGenBuffers(1, &tbo);

        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(float), textureCoords.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

public:

    LineRenderer(){
        this->renderMode = GL_TRIANGLES;
        this->startWidth = 1.0f;
        this->endWidth = 1.0f;
        glGenVertexArrays(1, &vao);
    }

    LineRenderer(LineRenderer * renderer){
        this->renderMode = renderer->renderMode;
        this->startWidth = renderer->startWidth;
        this->endWidth = renderer->endWidth;

        this->vao = renderer->vao;
        this->vbo = renderer->vbo;
        this->tbo = renderer->tbo;
        this->ebo = renderer->ebo;

        this->numIndices = renderer->numIndices;
        this->numVertices = renderer->numVertices;
    }

    void SetLineWidth(const float & startWidth, const float & endWidth) {
        this->startWidth = startWidth;
        this->endWidth = endWidth;
        RebuildQuads();
    }

    void SetRenderMode(const GLenum & mode){
        this->renderMode = mode;
    }

    AttributeID GetAttributeID() const override{
        return RenderingAttributes::LINE;
    }

    void SetVertices(const Vector3 * vertices, const unsigned int & numVertices){

        this->vertices.clear();

        for(int i =0; i < numVertices; i++)
            this->vertices.push_back(vertices[i]);

        RebuildQuads();
        RebuildTextureCoords();
    }

    void Draw(){

        glBindVertexArray(vao);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(renderMode, numIndices, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

    }

    ~LineRenderer(){
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &tbo);
        glDeleteVertexArrays(1, &vao);
    }

};

#endif
