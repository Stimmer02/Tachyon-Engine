#include "Component.h"
#include "Color.h"

Component::Component(const float &_x, const float &_y, const float &_width, const float &_height)
	:x(_x), y(_y), width(_width), height(_height){
}
bool Component::IsInBound(const float &_x,const float &_y){
	return (x <= _x) && (y <= _y) && (_x <= x + width) && (_y <= y + height);
}
void Component::SetColor(const Color &_color){
	color = _color;
}
GLuint Component::GetTextureID(){
	return texture;
}

GLuint Component::GetPBO(){
	return PBO;
}

float Component::getWidth(){
	return width;
}
float Component::getHeight(){
	return height;
}
float Component::getX(){
	return x;
}
float Component::getY(){
	return y;
}
Color Component::getColor(){
	return color;
}



