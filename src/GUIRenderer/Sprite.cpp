#include "Sprite.h"

Sprite::Sprite(const Color * pixels, const uint32_t& width, const uint32_t& height){

    if( pixels == nullptr )
        return;

    this->width = width;
    this->height = height;
    this->currentFrame = 0;
    this->UpdateTexture(pixels, width, height);

}

Sprite::Sprite(const char * filepath){

    std::map<std::string, GLuint>::iterator it = almanach.find( std::string(filepath) );

    if( it != almanach.end() ){
        frames.push_back( it->second );
        return;
    }

    Image img = BitmapReader::ReadFile(filepath);

    if( img.pixels == nullptr )
        return;

    this->width = img.width;
    this->height = img.height;
    this->currentFrame = 0;
    this->UpdateTexture(img.pixels, img.width, img.height);
    almanach[ std::string(filepath) ] = frames.back();

    delete[] img.pixels;

}

Sprite::Sprite(const Image * image){

    if( image->pixels == nullptr )
        return;

    this->width = image->width;
    this->height = image->height;
    this->currentFrame = 0;
    this->UpdateTexture(image->pixels, image->width, image->height);

}

Sprite::Sprite(const Sprite * sprite){

    this->width = sprite->width;
    this->height = sprite->height;
    this->currentFrame = 0;
    this->parent = sprite->parent;

    for( const GLuint & el : sprite->frames ){
        this->frames.push_back(el);
    }

}

void Sprite::UpdateTexture(const Color * pixels, const uint32_t& width, const uint32_t& height, const GLuint & frameID){

    GLuint textureID = 0;

    if( !frames.empty() && frames.size() > frameID)
        textureID = frames[frameID];

    if( textureID == 0){

        glGenTextures(1, &textureID);

        frames.emplace_back(textureID);

        // Select current texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Allocate texture buffer for pixels
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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

void Sprite::SetTextureAttrib(const GLenum & attrib, const GLint & value, const GLuint & frameID){

    if( frames.empty() || frames.size() <= frameID)
        return;

    glBindTexture(GL_TEXTURE_2D, frames[frameID]);
    glTexParameteri(GL_TEXTURE_2D, attrib, value);
    glBindTexture(GL_TEXTURE_2D, 0);
}

AttributeID Sprite::GetAttributeID() const{
    return RenderingAttributes::SPRITE;
}

void Sprite::Push(const Color * pixels, const uint32_t& width, const uint32_t& height){

    if( pixels == nullptr )
        return;

    GLuint textureID =  frames.size();

    this->UpdateTexture(pixels, width, height, textureID);

}

void Sprite::Push(const char * filepath){

    std::map<std::string, GLuint>::iterator it = almanach.find( std::string(filepath) );

    if( it != almanach.end() ){
        frames.push_back( it->second );
        return;
    }

    Image img = BitmapReader::ReadFile(filepath);

    if( img.pixels == nullptr )
        return;

    GLuint textureID = frames.size();

    this->UpdateTexture(img.pixels, img.width, img.height, textureID);
    almanach[ std::string(filepath) ] = frames.back();

    delete[] img.pixels;

}

void Sprite::Pop(){

    if( frames.size()<2 )
        return;

    GLuint textureID = frames.back();
    frames.pop_back();

    glDeleteTextures(1, &textureID);

}

void Sprite::SetPixel(const uint32_t & x, const uint32_t & y, const Color & color){

    if( x < 0 || x >= width || y < 0 || y >= height)
        return;

    glBindTexture(GL_TEXTURE_2D, frames[currentFrame]);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, height - y - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color);

    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Sprite::GetTextureID(const GLuint & frameID){

    if( frames.size() <= frameID )
        return - 1;

    return frames[frameID];
}

void Sprite::Load(){

    if( frames.empty() || currentFrame >= frames.size() ){
        defaultSprite->Load();
        return;
    }

    glBindTexture(GL_TEXTURE_2D, frames.front());
}

void Sprite::NextFrame(){

    currentFrame++;
    currentFrame *= ( currentFrame < frames.size() );

}

void Sprite::UnLoad(){
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::Destroy(){

    this->currentFrame = 0;
    for(GLuint frame : frames)
        glDeleteTextures(1, &frame);

}

void Sprite::ClearCanvas(const Color & clearColor){

    std::vector<Color> colors(width * height + width, clearColor);

    glBindTexture(GL_TEXTURE_2D, frames[currentFrame]);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, colors.data());

    glBindTexture(GL_TEXTURE_2D, 0);

}


Sprite::~Sprite(){
    this->Destroy();
}

