#ifndef CUBESPRITE_H
#define CUBESPRITE_H

#include "Sprite.h"

class CubeSprite : public Sprite{

private:

    GLuint texture = 0;
    uint32_t width = 0;
    uint32_t height = 0;

public:

    CubeSprite();

    CubeSprite(const CubeSprite * sprite);

    void UpdateFace(GLint face, const Color * pixels, const uint32_t& width, const uint32_t& height);

    void SetTextureAttrib(const GLenum & attrib, const GLint & value) override;

    GLuint GetTextureID() override;

    void SetPixel(const uint32_t & x, const uint32_t & y, const Color & color);

    void Load(GLShader * shader) override;

    void UnLoad() override;

    void ClearColor(const Color & clearColor) override;

    ~CubeSprite();

};


#endif
