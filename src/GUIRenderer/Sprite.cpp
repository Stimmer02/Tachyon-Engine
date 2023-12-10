#include "Sprite.h"

Sprite* Sprite::Create(const Color * pixels, const int& width, const int &height){

    Sprite *sprite = NULL;

    if(!pixels)
        return NULL;

    sprite = new Sprite();

    // Write texture informations
    sprite->width = width;
    sprite->height = height;

    // Calculate texture checksum
    sprite->CalculateChecksum(pixels, width, height);

    // Create texture name
    glGenTextures(1, &sprite->textureID);

    // Select current texture
    glBindTexture(GL_TEXTURE_2D, sprite->textureID);

    // Set texture wrapping mode
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Fill texture buffer with pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)pixels);

    // Set texture filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return sprite;
}

unsigned int Sprite::GetWidth(){
    return width;
}

unsigned int Sprite::GetHeight(){
    return height;
}

void Sprite::Load(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Sprite::UnLoad(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Sprite::Destroy(){
    glDeleteTextures(1, &textureID);
}

bool Sprite::operator==(const Sprite& sprite){

    char *check_other = ((Sprite)sprite).GetChecksum();

    unsigned char result = 0;

    result |= check_other[0] ^ checksum[0];
    result |= check_other[1] ^ checksum[1];
    result |= check_other[2] ^ checksum[2];
    result |= check_other[3] ^ checksum[3];

    return result==0;

}

char * Sprite::GetChecksum(){
    return (char*)checksum;
}

Sprite::~Sprite(){
    glDeleteTextures(1, &textureID);
}

Sprite::Sprite(){

}

void Sprite::CalculateChecksum(const Color * pixels, const int& width, const int &height){

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){

            unsigned int index = y * width + x;

            //Some checksum magic;

        }
    }

}
