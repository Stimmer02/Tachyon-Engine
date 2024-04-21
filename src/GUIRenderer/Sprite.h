#ifndef SPRITE_H
#define SPRITE_H

#include "BitmapReader.h"
#include "Attribute.h"

#include <stdio.h>
#include <map>
#include <string>
#include <cstring>

#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#elif _WIN32

#include <GL/glew.h>

#else

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>

#endif

static GLuint pixelBuffer;
static std::map<std::string, GLuint> almanach;

class Sprite : public AttributeType<Sprite>{

private:

    GLuint textureID = 0;

public:

    /// @brief Method creates a new Sprite within OpenGL graphic context.
    /// @param pixels
    /// @param width
    /// @param height
    Sprite(const Color * pixels, const uint32_t& width, const uint32_t& height);

    /// @brief Method creates a new Sprite within OpenGL graphic context.
    /// @param image
    /// @return Sprite object
    Sprite(const Image * image);

    /// @brief Method creates a new Sprite within OpenGL graphic context.
    /// @param path
    /// @return Sprite object
    Sprite(const char * filepath);

    /// @brief Method creates a new texture for current object.
    /// @param pixels
    /// @param width
    /// @param height
    void UpdateTexture(const Color * pixels, const uint32_t& width, const uint32_t& height);

    /// @brief Method returns texture id of current sprite
    /// @return Sprite texture id
    GLuint GetTextureID();

    /// @brief Method loads current sprite to texture buffer.
    void Load();

    /// @brief Method unloads current sprite from texture buffer.
    void UnLoad();

    /// @brief Method deletes texture from OpenGL graphic context.
    void Destroy();

    ~Sprite();

};


#endif
