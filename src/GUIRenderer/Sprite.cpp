#include "Sprite.h"
#include <stdio.h>


Sprite* Sprite::Create(const Color * pixels, const uint32_t& width, const uint32_t& height){

    Sprite *sprite = NULL;

    if( pixels == nullptr)
        return nullptr;

    sprite = new Sprite();

    // Write texture informations
    sprite->width = width;
    sprite->height = height;

    // Load data
    sprite->UpdateTexture(pixels, width, height);

    return sprite;
}

Sprite* Sprite::Create(const Image * image){
    Sprite *sprite = NULL;

    if( image->pixels == nullptr )
        return nullptr;

    sprite = new Sprite();

    // Write texture informations
    sprite->width = image->width;
    sprite->height = image->height;

    // Load data
    sprite->UpdateTexture(image->pixels, image->width, image->height);

    return sprite;
}

void Sprite::UpdateTexture(const Color * pixels, const uint32_t& width, const uint32_t& height){

    // Destroy old texture
    Destroy();

    // Calculate new texture checksum
    CalculateChecksum(pixels, width, height);

    // Create texture name
    glGenTextures(1, &textureID);

    // Select current texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Allocate texture buffer for pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unselect current texture
    glBindTexture(GL_TEXTURE_2D, 0);
}


GLuint Sprite::GetPixelBuffer(){

    // Unbind PBO from other texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind PBO to current texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    return pixelBuffer;
}

GLuint Sprite::GetTextureID(){
    return textureID;
}

uint32_t Sprite::GetWidth(){
    return width;
}

uint32_t Sprite::GetHeight(){
    return height;
}

void Sprite::Load(){
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Sprite::UnLoad(){
    glBindTexture(GL_TEXTURE_2D, 0);
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
    this->Destroy();
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
