#ifndef SPRITE_H
#define SPRITE_H

#include "Color.h"

#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#else

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>

#endif

class Sprite {

public:

    /// @brief Method creates a new Sprite within OpenGL graphic context.
    /// @param pixels
    /// @param width
    /// @param height
    /// @return Sprite object
    static Sprite* Create(const Color * pixels, const uint32_t& width, const uint32_t& height);

    /// @brief Method returns width of sprite.
    /// @return Sprite width
    uint32_t GetWidth();

    /// @brief Method returns height of sprite.
    /// @return Sprite height
    uint32_t GetHeight();

    /// @brief Method returns sprite cheksum.
    /// @return Sprite checksum
    uint32_t GetChecksum();

    /// @brief Method loads current sprite to texture buffer.
    void Load();

    /// @brief Method unloads current sprite from texture buffer.
    void UnLoad();

    /// @brief Method deletes texture from OpenGL graphic context.
    void Destroy();

    bool operator==(const Sprite& sprite);

    ~Sprite();

private:

    Sprite();

    /// @brief Method calculate code image within uint32_t range
    /// @param pixels
    /// @param width
    /// @param height
    void CalculateChecksum(const Color * pixels, const uint32_t& width, const uint32_t& height);

    uint32_t width;
    uint32_t height;

    GLuint textureID;

    uint32_t checksum;

};


#endif
