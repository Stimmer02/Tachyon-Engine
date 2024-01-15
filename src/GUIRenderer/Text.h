#ifndef TEXT_H
#define TEXT_H

#include "Component.h"
#include <iostream>

class Text : public Component{
private:
	char* text;
	unsigned int textLen;
public:
	Text(float _x, float _y, float _width, float _height, char* _text, unsigned int _textLen);
	void Render();
};

#endif
