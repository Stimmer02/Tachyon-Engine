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

}

void CubeSprite::UnLoad(){

}

void CubeSprite::ClearColor(const Color & clearColor){

}

CubeSprite::~CubeSprite(){

}
