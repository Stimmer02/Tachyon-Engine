#ifndef GLYF_H
#define GLYF_H

#include "Sprite.h"
#include "Vector3.h"

#include <cassert>
#include "Headers.h"

/// @brief This class represents single character
struct Glyf{

private:

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint tbo = 0;

    void AllocateBuffers(){

        const Vector3 vertices[] = {
            { -width * 0.5f, -height * 0.5f, 0.0f },
            { width * 0.5f, -height * 0.5f, 0.0f },
            { width * 0.5f, height * 0.5f, 0.0f},
            { -width * 0.5f, height * 0.5f, 0.0f }
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

    Sprite * sprite;

    char character;
    int width;
    int height;

    Glyf(const char & character, const int & width, const int & height, Sprite * sprite){
        assert(sprite != nullptr && "Texture cannot be null");

        this->character = character;
        this->width = width;
        this->height = height;
        this->sprite = sprite;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &tbo);

        AllocateBuffers();
    }

    void Render(){

        sprite->Load(currentShader);

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);

        sprite->UnLoad();

    }

    ~Glyf(){
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &tbo);
        glDeleteVertexArrays(1, &vao);

        delete sprite;

    }

};



#endif
