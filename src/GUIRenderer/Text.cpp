#include "Text.h"

Text::Text(float _x, float _y, float _width, float _height, char* _text, unsigned int _textLen)
	:Component(_x, _y, _width, _height), text(_text), textLen(_textLen){
}
void Text::Render(){
	std::cout << "Tutaj powstanie kod odpowiadający za renderowanie textu ale teraz go nie ma bo nie\n";
}
