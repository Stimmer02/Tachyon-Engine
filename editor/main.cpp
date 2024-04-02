#include "GraphicSystem.h"
#include "GLShader.h"

#include "Vertex.h"

class Quad{
private:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    vertex vertices[4];

public:

    Quad(float x, float y){

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        vertices[0] = {-5 + x, -5 + y, 0.0f};
        vertices[1] = { 5 + x, -5 + y, 0.0f};
        vertices[2] = { 5 + x,  5 + y, 0.0f};
        vertices[3] = {-5 + x,  5 + y, 0.0f};

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), vertices, GL_STATIC_DRAW);

        GLuint indices[6] = {0, 1, 3, 1, 2, 3};

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));

        glBindVertexArray(0);

    }

    void Draw() const{

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    ~Quad(){
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

};

void Render();

void Ortho(float matrix[16], const float & left, const float & right, const float &bottom, const float & top, const float & near, const float & far);

std::vector<Quad*> quads;

int main(){

    // Create system
    GraphicSystem graphic(Render);

    // Create shader
    GLShader mainShader;
    mainShader.LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
    mainShader.LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
    mainShader.Build();

    // Locate model in shader
    GLint modelLocation = mainShader.GetUniformLocation("model");

    // Fill model matrix
    float model[16] = {};
    Ortho(model, 0, 800, 0, 600, -1.0f, 1.0f);

    // Create objects

    srand(time(NULL));

    for (int i=0; i<100; i++){

        float x = (( rand()/(float)RAND_MAX ) * 2.0f - 1.0f) * 100.0f + 400;
        float y = (( rand()/(float)RAND_MAX ) * 2.0f - 1.0f) * 100.0f + 300;

        Quad *q = new Quad(x, y);

        quads.emplace_back(q);

    }

    // Enable shader
    mainShader.Use();

    // Transfer model
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model);

    graphic.Run();

    mainShader.Dispose();

    for(Quad * quad : quads)
        delete quad;

    return 0;
}

void Ortho(float matrix[16], const float & left, const float & right, const float &bottom, const float & top, const float & near, const float & far){

    matrix[0] = 2.0f / (right - left);
    matrix[3] = - (right+left)/(right-left);
    matrix[5] = 2.0f / (top - bottom);
    matrix[7] = - (top+bottom)/(top-bottom);
    matrix[10] = -2.0f /(far - near);
    matrix[11] = - (far + near)/(far - near);
    matrix[15] = 1.0f;

}

void Render(){

    for(Quad * quad : quads){
        quad->Draw();
    }

}

