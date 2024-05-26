#include "MultiSprite.h"


MultiSprite::MultiSprite() : Sprite(){
    textures.clear();
}

MultiSprite::MultiSprite(const MultiSprite * sprite) : Sprite(){

    for( const auto & p : sprite->textures )
        this->textures[p.first] = new Sprite2D(p.second);

}

void MultiSprite::SetTexture(const std::string & texName, const Color * pixels, const uint32_t& width, const uint32_t& height){

    auto it = textures.find(texName);

    Sprite2D * sprite = nullptr;

    if( it == textures.end() ){
        sprite = new Sprite2D(pixels, width, height);
        textures[texName] = sprite;
    }else{
        sprite = it->second;
        sprite->UpdateTexture(pixels, width, height);
    }

}

void MultiSprite::SetTextureAttrib(const GLenum & attrib, const GLint & value){

    if( textures.empty() )
        return;

    for( auto & p : textures)
        p.second->SetTextureAttrib(attrib, value);

}

GLuint MultiSprite::GetTextureID(){
    if( textures.empty() )
        return 0;

    return textures.begin()->second->GetTextureID();
}

void MultiSprite::Load(GLShader * shader){

    int i = 0;
    for( auto & p : textures){
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, p.second->GetTextureID());
        shader->TransferToShader(p.first, i);
        i++;
    }

}

void MultiSprite::UnLoad(){

    int i = 0;
    for (auto & p : textures) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
        i++;
    }
    glActiveTexture(GL_TEXTURE0);

}

void MultiSprite::ClearColor(const Color & clearColor){

    for( auto & p : textures)
        p.second->ClearColor(clearColor);

}

MultiSprite::~MultiSprite(){

    for( auto & p : textures)
        delete p.second;

}
