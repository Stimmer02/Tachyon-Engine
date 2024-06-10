#include "CubeSprite.h"


CubeSprite::CubeSprite(){

}

CubeSprite::CubeSprite(const CubeSprite * sprite){



}

void CubeSprite::UpdateFace(GLint face, const Color * pixels, const uint32_t& width, const uint32_t& height){

}

void CubeSprite::SetTextureAttrib(const GLenum & attrib, const GLint & value){

}

GLuint CubeSprite::GetTextureID(){
    return texture;
}


void CubeSprite::Load(GLShader * shader){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void CubeSprite::UnLoad(){
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeSprite::ClearColor(const Color & clearColor){

}

CubeSprite::~CubeSprite(){
    glDeleteTextures(1, &texture);
}
