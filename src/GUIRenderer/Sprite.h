#ifndef SPRITE_H
#define SPRITE_H

#include "BitmapReader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <vector>

static std::map<std::string, GLuint> almanach;

class Sprite{

public:

    /// @brief Method sets value correlated to texture attribute
    virtual void SetTextureAttrib(const GLenum & attrib, const GLint & value) = 0;

    /// @brief Method returns texture id of current sprite
    /// @return Sprite texture id
    virtual GLuint GetTextureID() = 0;

    /// @brief Method loads current sprite to texture buffer.
    virtual void Load() = 0;

    /// @brief Method unloads current sprite from texture buffer.
    virtual void UnLoad() = 0;

    /// @brief Method fills texture with provided color
    virtual void ClearColor(const Color & clearColor) = 0;

    virtual ~Sprite(){};

};

static Sprite * defaultSprite;

#endif
