#ifndef BUTTONELEMENT_H
#define BUTTONELEMENT_H

#include "Sprite.h"
#include "InteractiveElement.h"
#include "GLShader.h"

class ButtonElement : public InteractiveElement{

private:

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint tbo = 0;

    Sprite * texture;

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

    ButtonElement(const int width, const int height, const Callback callbackFunc) : InteractiveElement(width, height, callbackFunc) {

        this->transform.position.x = 0;
        this->transform.position.y = 0;
        this->texture = defaultSprite;

        Matrix translation = Matrix::Translate(transform.position.x, transform.position.y, transform.position.z);
        Matrix rotation = Matrix::Rotate(transform.rotation);
        Matrix scale = Matrix::Scale(transform.scale.x, transform.scale.y, transform.scale.z);

        model = translation * rotation * scale;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &tbo);

        AllocateBuffers();
    }

    void SetTexture(Sprite * texture){
        this->texture = texture;
    }

    void Render() override{

        GLShader * shader = currentShader;

        shader->TransferToShader("u_model", model);

        texture->Load(shader);

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);

        texture->UnLoad();

    }

    ~ButtonElement(){
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &tbo);
        glDeleteVertexArrays(1, &vao);
    }

};


#endif
