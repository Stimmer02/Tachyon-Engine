#ifndef COMPONENT_H
#define COMPONENT_H

#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#elif _WIN32

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#else

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>

#endif

#include "Color.h"
#include "Sprite.h"

class Component{
protected:

	float height;
	float width;
	float x;
	float y;

	Color color;
	Sprite * sprite;

public:
	Component(const float &_x, const float &_y, const float &_width, const float &_height);

	bool IsInBound(const float &_x,const float &_y);

	void SetColor(const Color &_color);

	virtual void Render() = 0;

	void SetTexture( const Sprite * _sprite);

	Sprite GetTexture();

	float getWidth();

	float getHeight();

	float getX();

	float getY();

	Color getColor();

	bool operator ==(const Component &other);
};

#endif
