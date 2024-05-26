#ifndef MULTISPRITE_H
#define MULTISPRITE_H

#include "Sprite2D.h"
#include "GLShader.h"

class MultiSprite : public Sprite{
private:

    std::map< std::string, Sprite2D* > textures;

public:

    MultiSprite();

    MultiSprite(const MultiSprite * sprite);

    void SetTexture(const std::string & texName, const Color * pixels, const uint32_t& width, const uint32_t& height);

    void SetTextureAttrib(const GLenum & attrib, const GLint & value) override;

    GLuint GetTextureID() override;

    void Load(GLShader * shader) override;

    void UnLoad() override;

    void ClearColor(const Color & clearColor) override;

    ~MultiSprite();

};



#endif
