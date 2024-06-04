#ifndef SPRITE2D_H
#define SPRITE2D_H

#include "Sprite.h"

class Sprite2D : public Sprite{

private:

    GLuint texture = 0;
    uint32_t width = 0;
    uint32_t height = 0;

public:

    Sprite2D(const Color * pixels, const uint32_t& width, const uint32_t& height);

    Sprite2D(const Image * image);

    Sprite2D(const char * filepath);

    Sprite2D(const Sprite2D * sprite);

    void UpdateTexture(const Color * pixels, const uint32_t& width, const uint32_t& height);

    void SetTextureAttrib(const GLenum & attrib, const GLint & value) override;

    GLuint GetTextureID() override;

    void SetPixel(const uint32_t & x, const uint32_t & y, const Color & color);

    void Load(GLShader * shader) override;

    void UnLoad() override;

    void ClearColor(const Color & clearColor) override;

    ~Sprite2D();

};


#endif
