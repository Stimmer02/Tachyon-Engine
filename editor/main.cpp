#include "GraphicSystem.h"
#include "GLShader.h"
#include "TransformStack.h"
#include "Vertex.h"

#include <chrono>

class Component{
private:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    vertex vertices[4];

public:

    Component(float x, float y){

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

    ~Component(){
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

};

void Render();

float xOff, yOff, zOff;

GLint modelLocation;
std::vector<Component*> components;

int main(){

    // Initialize affine stack
    TransformStack::Initialize();
    TransformStack::Push();
    TransformStack::Ortho(0, 800, 0, 600, -100.0f, 100.0f);

    // Create system
    GraphicSystem graphic(Render);

    // Create shader
    GLShader mainShader;
    mainShader.LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
    mainShader.LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
    mainShader.Build();

    // Locate model in shader
    modelLocation = mainShader.GetUniformLocation("model"); 

    // Create objects

    srand(time(NULL));

    for (int i=0; i<1000; i++){

        float x = cos(( rand()/(float)RAND_MAX ) * 2.0f * 3.1415926535f) * 100.0f + 400;
        float y = sin(( rand()/(float)RAND_MAX ) * 2.0f * 3.1415926535f) * 100.0f + 300;

        Component *q = new Component(x, y);

        components.emplace_back(q);

    }

    // Enable shader
    mainShader.Use();

    graphic.Run();

    mainShader.Dispose();

    for(Component * Component : components)
        delete Component;

    return 0;
}


void Render(){

    static std::chrono::high_resolution_clock::time_point last;
    static int frames;
    static float accumulated;

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    frames++;


    float delta = std::chrono::duration_cast< std::chrono::duration<float> >(now - last).count();

    if( delta >= 1.0f){
        printf("FPS : %d\r", frames);
        frames = 0;
        last = now;
    }

    TransformStack::Push();
    TransformStack::Translate(400, 300, 0);
    TransformStack::Rotate(xOff, xOff, yOff, 1.0f);
    TransformStack::Translate(-400, -300, 0);
    Matrix model = TransformStack::Top();
    TransformStack::Pop();

    // Transfer model
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.Data());

    for(Component * component : components){
        component->Draw();
    }

    xOff = cos(zOff);
    yOff = sin(zOff);
    zOff += 0.1f;
}

