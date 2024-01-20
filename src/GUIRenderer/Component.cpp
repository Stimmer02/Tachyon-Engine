#include "Component.h"
#include "Color.h"

Component::Component(const float &_x, const float &_y, const float &_width, const float &_height){
	this->x = _x;
	this->y = _y;
	this->width= _width;
	this->height = _height;
}

bool Component::IsInBound(const float & _x, const float & _y){
	
	float w_2 = width/2.0f;
	float h_2 = height/2.0f;

	return  ( ( x - w_2 ) <= _x ) && ( _x <= ( x + w_2 ) ) && ( ( y - h_2 ) <= _y ) && ( _y <= ( y + h_2 ) );

}

void Component::SetColor(const Color &_color){
	color = _color;
}

void Component::Render(){

	glColor3b(255, 255, 255);

	float w_2 = width/2.0f;
	float h_2 = height/2.0f;

	glBegin(GL_QUADS);
		glVertex2f( x - w_2, y - h_2);
		glVertex2f( x + w_2, y - h_2);
		glVertex2f( x + w_2, y + h_2);
		glVertex2f( x - w_2, y + h_2);
	glEnd();

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



