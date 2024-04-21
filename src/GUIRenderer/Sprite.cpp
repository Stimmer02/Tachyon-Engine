#include "Sprite.h"

Sprite::Sprite(const Color * pixels, const uint32_t& width, const uint32_t& height){

    if( pixels == nullptr )
        return;

    this->UpdateTexture(pixels, width, height);
}

Sprite::Sprite(const char * filepath){

    std::map<std::string, GLuint>::iterator it = almanach.find( std::string(filepath) );

    if( it != almanach.end() ){
        this->textureID = it->second;
        return;
    }

    Image img = BitmapReader::ReadFile(filepath);

    if( img.pixels == nullptr )
        return;

    this->UpdateTexture(img.pixels, img.width, img.height);
    almanach[ std::string(filepath) ] = textureID;

    delete[] img.pixels;

}

Sprite::Sprite(const Image * image){

    if( image->pixels == nullptr )
        return;

    this->UpdateTexture(image->pixels, image->width, image->height);

}

void Sprite::UpdateTexture(const Color * pixels, const uint32_t& width, const uint32_t& height){


    if( this->textureID == 0){

        glGenTextures(1, &textureID);

#ifdef DEBUG

        printf("[DEBUG] Generating texture %d\n", textureID);

#endif

        // Select current texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Allocate texture buffer for pixels
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        // Set texture filtering mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    }else{

        // Select current texture
        glBindTexture(GL_TEXTURE_2D, textureID);

#ifdef DEBUG

        printf("[DEBUG] Updating texture %d\n", textureID);

#endif
        // Set texture filtering mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);


    }

    // Unselect current texture
    glBindTexture(GL_TEXTURE_2D, 0);

}

GLuint Sprite::GetTextureID(){
    return textureID;
}

void Sprite::Load(){
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Sprite::UnLoad(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::Destroy(){
    glDeleteTextures(1, &textureID);
    textureID = 0;
}

Sprite::~Sprite(){
    this->Destroy();
}

