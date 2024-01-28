#include "Sprite.h"
#include <stdio.h>

Sprite* Sprite::Create(const Color * pixels, const uint32_t& width, const uint32_t& height){

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)pixels);

    // Set texture filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return sprite;
}

Sprite* Sprite::Create(const Image * image){
    Sprite *sprite = NULL;

    if(!image->pixels)
        return NULL;

    sprite = new Sprite();

    // Write texture informations
    sprite->width = image->width;
    sprite->height = image->height;

    // Calculate texture checksum
    sprite->CalculateChecksum(image->pixels, image->width, image->height);

    // Create texture name
    glGenTextures(1, &sprite->textureID);

    // Select current texture
    glBindTexture(GL_TEXTURE_2D, sprite->textureID);

    // Set texture wrapping mode
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Fill texture buffer with pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)image->pixels);

    // Set texture filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return sprite;
}

uint32_t Sprite::GetWidth(){
    return width;
}

uint32_t Sprite::GetHeight(){
    return height;
}

uint32_t Sprite::GetTexture(){
    return textureID;
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

    uint32_t checksumOther = ((Sprite)sprite).GetChecksum();

    return !(checksumOther^checksum);

}

uint32_t Sprite::GetChecksum(){
    return checksum;
}

Sprite::~Sprite(){
    glDeleteTextures(1, &textureID);
}

Sprite::Sprite(){
    this->checksum = 0;
}

void Sprite::CalculateChecksum(const Color * pixels, const uint32_t& width, const uint32_t& height){

    const uint8_t corners[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    uint32_t tempChecksum = 0;

    for(uint32_t index=0; index<(width*height); index++){

        uint8_t * bytes = (uint8_t *)&pixels[index];

        for(uint8_t byteIndex=0; byteIndex<4; ++byteIndex){

            uint8_t minDifference = 255;
            uint8_t minDifferenceIndex = 0;

            for(uint8_t i=0; i < 8; i++){

                uint8_t difference = corners[i] & bytes[byteIndex];

                if(difference < minDifference){
                    minDifference = difference;
                    minDifferenceIndex = i;
                }

            }

            tempChecksum ^= bytes[byteIndex];
            tempChecksum <<= corners[minDifferenceIndex];
            tempChecksum |= bytes[byteIndex];

        }

        checksum ^= tempChecksum;
    }

}
