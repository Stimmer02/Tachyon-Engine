#ifndef SPRITE_H
#define SPRITE_H

#include "BitmapReader.h"
#include "GLShader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <vector>

class Sprite{

protected:

    GLenum sfactor;
    GLenum dfactor;
    bool isTransparent = false;

public:

    /// @brief Method sets value correlated to texture attribute
    virtual void SetTextureAttrib(const GLenum & attrib, const GLint & value) = 0;

    /// @brief Method returns texture id of current sprite
    /// @return Sprite texture id
    virtual GLuint GetTextureID() = 0;

    /// @brief Method loads current sprite to texture buffer.
    virtual void Load(GLShader * shader) = 0;

    /// @brief Method unloads current sprite from texture buffer.
    virtual void UnLoad() = 0;

    /// @brief Method enables transparency for the sprite with specific blend function
    /// @param sfactor Specifies how the red, green, blue, and alpha source blending factors are computed. The initial value is GL_SRC_ALPHA.
    /// @param dfactor Specifies how the red, green, blue, and alpha destination blending factors are computed. The initial value is GL_ONE_MINUS_SRC_ALPHA.
    void EnableTransparency(GLenum sfactor = GL_SRC_ALPHA, GLenum dfactor = GL_ONE_MINUS_SRC_ALPHA) {
        isTransparent = true;
        this->sfactor = sfactor;
        this->dfactor = dfactor;
    }

    /// @brief Method disables transparency for the sprite
    void DisableTransparency() {
        isTransparent = false;
    }

    /// @brief Method fills texture with provided color
    virtual void ClearColor(const Color & clearColor) = 0;

    virtual ~Sprite(){};

};

static std::map<std::string, Sprite *> almanach;
static Sprite * defaultSprite;

#endif
