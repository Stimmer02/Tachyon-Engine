#include "Canvas.h"

Canvas::Canvas(const int& x, const int& y, const int& width, const int& height) : InteractiveComponent(x, y, width, height){

}

void Canvas::Render(){
    if(sprite != nullptr)
		sprite->Load();

	float w_2 = width/2.0f;
	float h_2 = height/2.0f;

	glBegin(GL_QUADS);
		glTexCoord2i(0, 1); glVertex2f( x - w_2, y - h_2);
		glTexCoord2i(1, 1); glVertex2f( x + w_2, y - h_2);
		glTexCoord2i(1, 0); glVertex2f( x + w_2, y + h_2);
		glTexCoord2i(0, 0); glVertex2f( x - w_2, y + h_2);
	glEnd();

	if(sprite != nullptr)
		sprite->UnLoad();
}
