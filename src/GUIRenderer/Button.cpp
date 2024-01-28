#include "Button.h"


Button::Button(const int& x, const int& y, const int& width, const int& height) : InteractiveComponent(x,y,width,height){

	float w_2 = width/2.0f;
	float h_2 = height/2.0f;

	float vertices[16] = {0};
	vertices[0] = x - w_2;
	vertices[1] = y - h_2;
	vertices[2] = 0.0f;
	vertices[3] = 1.0f;

	vertices[4] = x + w_2;
	vertices[5] = y - h_2;
	vertices[6] = 1.0f;
	vertices[7] = 1.0f;

	vertices[8] = x + w_2;
	vertices[9] = y + h_2;
	vertices[10] = 1.0f;
	vertices[11] = 0.0f;

	vertices[12] = x - w_2;
	vertices[13] = y + h_2;
	vertices[14] = 0.0f;
	vertices[15] = 0.0f;

	glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Button::Render(){

	if(sprite != nullptr)
		sprite->Load();

		// Bind the VAO
        glBindVertexArray(VAO);

        // Draw the quad
        glDrawArrays(GL_TRIANGLES, 0, 4);

        // Unbind the VAO and texture
        glBindVertexArray(0);

	if(sprite != nullptr)
		sprite->UnLoad();
}
