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

    static Sprite* Create(const Color * pixels, const uint32_t& width, const uint32_t& height);

    uint32_t GetWidth();
    uint32_t GetHeight();
    uint32_t GetChecksum();

    void Load();
    void UnLoad();

    void Destroy();

    bool operator==(const Sprite& sprite);

    ~Sprite();

private:

    Sprite();

    void CalculateChecksum(const Color * pixels, const uint32_t& width, const uint32_t& height);

    uint32_t width;
    uint32_t height;

    GLuint textureID;

    uint32_t checksum;

};


#endif
