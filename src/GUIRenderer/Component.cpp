#include "Component.h"

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

void Component::SetTexture( const Sprite2D * _sprite){
	sprite = (Sprite2D*)_sprite;;
}

Sprite2D Component::GetTexture(){
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

bool Component::operator ==(const Component &other){
	if(x == other.x && y == other.y && width == other.width && height == other.height){
		return true;
	}
	return false;
}



