#ifndef COMPONENT_H
#define COMPONENT_H

#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#else

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>

#endif


#include "Color.h"

class Component{
protected:
	float height, width, x, y;
	Color color;
	GLuint texture;
	GLuint PBO;
public:
	Component(const float &_x, const float &_y, const float &_width, const float &_height);
	bool IsInBound(const float &_x,const float &_y);
	void SetColor(const Color &_color);
	virtual void Render() = 0;
	GLuint GetTextureID();
	GLuint GetPBO();
};

#endif
