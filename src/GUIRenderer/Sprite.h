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

#include <vector>


static std::map<std::string, GLuint> almanach;

class Sprite : public AttributeType<Sprite>{

private:

    std::vector<GLuint> frames;
    int currentFrame = 0;

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
    void UpdateTexture(const Color * pixels, const uint32_t& width, const uint32_t& height, const GLuint & frameID = 0);

    /// @brief Method sets value correlated to texture attribute
    void SetTextureAttrib(const GLenum & attrib, const GLint & value, const GLuint & frameID = 0);

    /// @brief Method returns texture id of current sprite
    /// @return Sprite texture id
    GLuint GetTextureID(const GLuint & frameID = 0);

    /// @brief Pushes a new frame to animation
    /// @param pixels
    /// @param width
    /// @param height
    void Push(const Color * pixels, const uint32_t& width, const uint32_t& height);

    /// @brief Pushes a new frame to animation
    /// @param filepath
    void Push(const char * filepath);

    /// @brief Switches pointer to the next frame
    void NextFrame();

    /// @brief Removes the last frame from animation
    void Pop();

    /// @brief Method loads current sprite to texture buffer.
    void Load();

    /// @brief Method unloads current sprite from texture buffer.
    void UnLoad();

    /// @brief Method deletes texture from OpenGL graphic context.
    void Destroy();

    AttributeID GetAttributeID() const override;

    ~Sprite();

};

static Sprite * defaultSprite;

#endif
