#include "Sprite2D.h"

Sprite2D::Sprite2D(const Color * pixels, const uint32_t& width, const uint32_t& height) : Sprite(){

    if( pixels == nullptr )
        return;

    this->width = width;
    this->height = height;
    this->UpdateTexture(pixels, width, height);
}

Sprite2D::Sprite2D(const char * filepath) : Sprite(){

    Image img = BitmapReader::ReadFile(filepath);

    if( img.pixels == nullptr )
        return;

    this->width = img.width;
    this->height = img.height;
    this->UpdateTexture(img.pixels, img.width, img.height);
    almanach[ std::string(filepath) ] = texture;

    delete[] img.pixels;

}

Sprite2D::Sprite2D(const Image * image) : Sprite(){

    if( image->pixels == nullptr )
        return;

    this->width = image->width;
    this->height = image->height;
    this->UpdateTexture(image->pixels, image->width, image->height);

}

Sprite2D::Sprite2D(const Sprite2D * sprite) : Sprite(){
    this->width = sprite->width;
    this->height = sprite->height;
    this->texture = sprite->texture;
}

void Sprite2D::UpdateTexture(const Color * pixels, const uint32_t& width, const uint32_t& height){

    if( texture == 0){

        // Create handle
        glGenTextures(1, &texture);

        // Select current texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // Allocate texture buffer for pixels
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    }else{

        // Select current texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    }

    // Set texture filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Unselect current texture
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Sprite2D::SetTextureAttrib(const GLenum & attrib, const GLint & value){

    if( texture == 0 )
        return;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, attrib, value);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite2D::SetPixel(const uint32_t & x, const uint32_t & y, const Color & color){

    if( x < 0 || x >= width || y < 0 || y >= height)
        return;

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, height - y - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color);

    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Sprite2D::GetTextureID(){
    return texture;
}

void Sprite2D::Load(){
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Sprite2D::UnLoad(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite2D::ClearColor(const Color & clearColor){

    std::vector<Color> colors(width * height + width, clearColor);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, colors.data());
    glBindTexture(GL_TEXTURE_2D, 0);

}


Sprite2D::~Sprite2D(){
    glDeleteTextures(1, &texture);
}

