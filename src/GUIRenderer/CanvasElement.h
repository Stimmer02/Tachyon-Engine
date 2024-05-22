#ifndef CANVASELEMENT_H
#define CANVASELEMENT_H

#include "InteractiveElement.h"
#include "Sprite.h"
#include "GLShader.h"

class CanvasElement : public InteractiveElement{

private:

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint tbo = 0;

    Sprite * texture = nullptr;
    int cellSize = 10;

    void AllocateBuffers(){

        const Vector3 vertices[] = {
            { 0.0f, 0.0f, 0.0f },
            { width, 0.0f, 0.0f },
            { width, height, 0.0f},
            { 0.0f, height, 0.0f }
        };

        const float uvs[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vector3), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), uvs, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

    }

public:

    CanvasElement(const int & x, const int & y, const int & width, const int & height) : InteractiveElement(width, height, nullptr){

        this->transform.position.x = x;
        this->transform.position.y = y;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &tbo);

        AllocateBuffers();

        Realloc();

    }

    void SetCellSize(const int & cellSize){
        this->cellSize = cellSize;
        Realloc();
    }

    bool isInBound(const float & x, const float & y) const{
        return x >= transform.position.x && x <= transform.position.x + width &&
               y >= transform.position.y && y <= transform.position.y + height;
    }

    void SetPixel(const float & x , const float & y, const Color & color){

        int pixelX = static_cast<int>(x - transform.position.x)/cellSize;
        int pixelY = static_cast<int>(y - transform.position.y)/cellSize;

        texture->SetPixel(pixelX, pixelY, color);

    }

    void UploadImage(const Image & image){
        if( image.width > this->width || image.height > this->height || image.pixels == nullptr)
            return;
        texture->UpdateTexture(image.pixels, image.width, image.height);
    }

    void Realloc(){
        if( texture != nullptr )
            delete texture;

        std::vector<Color> colors;

        const int texW = (width + cellSize - 1) / cellSize;
        const int texH = (height + cellSize - 1) / cellSize;

        for(int i = 0; i < (texW * texH + width); i++)
            colors.push_back({255,255,255});

        texture = new Sprite(colors.data(), texW, texH);
    }

    void ClearCanvas(const Color & clearColor) {

        texture->ClearCanvas(clearColor);

    }

    void Render() override{

        GLShader * shader = currentShader;

        shader->TransferToShader("u_model", model);

        texture->Load();

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);

        texture->UnLoad();

    }

    ~CanvasElement(){
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &tbo);
        glDeleteVertexArrays(1, &vao);
        delete texture;
    }

};

#endif
