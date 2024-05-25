#ifndef SPRITE_H
#define SPRITE_H

#include "BitmapReader.h"

#include <stdio.h>
#include <map>
#include <string>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>


static std::map<std::string, GLuint> almanach;

class Sprite{

private:

    std::vector<GLuint> frames;
    int currentFrame = 0;
    uint32_t width;
    uint32_t height;

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

    /// @brief Method creates deep copy of given sprite
    Sprite(const Sprite * sprite);

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

    /// @brief Changes color of pixel at given coordinates
    void SetPixel(const uint32_t & x, const uint32_t & y, const Color & color);

    /// @brief Method loads current sprite to texture buffer.
    void Load();

    /// @brief Method unloads current sprite from texture buffer.
    void UnLoad();

    /// @brief Method deletes texture from OpenGL graphic context.
    void Destroy();

    /// @brief Method fills texture with provided color
    void ClearCanvas(const Color & clearColor);

    ~Sprite();

};

static Sprite * defaultSprite;

#endif
