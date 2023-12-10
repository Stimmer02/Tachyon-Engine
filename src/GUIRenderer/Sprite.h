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

    static Sprite* Create(const Color * pixels, const int& width, const int &height);

    unsigned int GetWidth();
    unsigned int GetHeight();
    char * GetChecksum();

    void Load();
    void UnLoad();

    void Destroy();

    bool operator==(const Sprite& sprite);

    ~Sprite();

private:

    Sprite();

    void CalculateChecksum(const Color * pixels, const int& width, const int &height);

    unsigned int width;
    unsigned int height;

    GLuint textureID;

    unsigned int checksum[4];

};


#endif
