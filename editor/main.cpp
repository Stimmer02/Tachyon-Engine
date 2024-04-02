#include "GraphicSystem.h"
#include "GLShader.h"

int main(){

    GraphicSystem graphic;

    GLShader mainShader;
    mainShader.LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
    mainShader.LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
    mainShader.Build();

    mainShader.Use();

    graphic.Run();

    mainShader.Dispose();

    return 0;
}
