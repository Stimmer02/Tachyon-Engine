#ifndef COMPONENT_H
#define COMPONENT_H

#include "Headers.h"

#include "Color.h"
#include "Sprite2D.h"

class Component{
protected:

	float height;
	float width;
	float x;
	float y;

	Color color;
	Sprite2D * sprite;

public:
	Component(const float &_x, const float &_y, const float &_width, const float &_height);

	bool IsInBound(const float &_x,const float &_y);

	void SetColor(const Color &_color);

	virtual void Render() = 0;

	void SetTexture( const Sprite2D * _sprite);

	Sprite2D GetTexture();

	float getWidth();

	float getHeight();

	float getX();

	float getY();

	Color getColor();

	bool operator ==(const Component &other);
};

#endif
