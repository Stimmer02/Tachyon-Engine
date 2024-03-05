#include "Component.h"

Component::Component(){
	this->x = 0;
	this->y = 0;
	this->width= 0;
	this->height = 0;
}

Component::Component(float _x, float _y, float _width, float _height, float xyx){
 	this->x = _x;
	this->y = _y;
	this->width= _width;
	this->height = _height;
}

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

void Component::SetTexture( const Sprite * _sprite){
	sprite = (Sprite*)_sprite;;
}

Sprite Component::GetTexture(){
	return *sprite;
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



