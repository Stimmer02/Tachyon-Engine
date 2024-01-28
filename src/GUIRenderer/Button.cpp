#include "Button.h"


Button::Button(const int& x, const int& y, const int& width, const int& height) : InteractiveComponent(x,y,width,height){

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Button::Render(){

	if(sprite != nullptr)
		sprite->Load();

        // glBindVertexArray(VAO);

        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // glBindVertexArray(0);

	if(sprite != nullptr)
		sprite->UnLoad();
}
