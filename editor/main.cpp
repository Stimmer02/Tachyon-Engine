#include "GraphicSystem.h"
#include "InteractionSystem.h"
#include "GLShader.h"
#include "TransformStack.h"
#include "Vertex.h"

#include "BitmapReader.h"
#include "Sprite.h"

#include <chrono>
#include <array>

class Component{
private:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    Sprite * sprite;

    std::array<vertex, 4> vertices;

public:

    float xO, yO;

    Component(float x, float y){

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        vertices[0] = {-5 + x, -5 + y, 0.0f};
        vertices[1] = { 5 + x, -5 + y, 0.0f};
        vertices[2] = { 5 + x,  5 + y, 0.0f};
        vertices[3] = {-5 + x,  5 + y, 0.0f};

        vertices[0].uvs = { 0.0f, 0.0f};
        vertices[1].uvs = { 1.0f, 0.0f};
        vertices[2].uvs = { 1.0f, 1.0f};
        vertices[3].uvs = { 0.0f, 1.0f};

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

        GLuint indices[6] = {0, 1, 3, 1, 2, 3};

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uvs));

        glBindVertexArray(0);

        Image image = BitmapReader::ReadFile("resources/sprites/slime.bmp");
        sprite = Sprite::Create(&image);

        delete[] image.pixels;

    }

    void Draw() const{

        sprite->Load();

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        sprite->UnLoad();

    }

    ~Component(){

        delete sprite;

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

};

class Button {
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    Sprite * sprite;

    std::array<vertex, 4> vertices;
    std::function<void()> onClick;

public:
    Button(float x, float y, float width, float height){

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        vertices[0] = {-5 + x, -5 + y, 0.0f};
        vertices[1] = { 5 + x, -5 + y, 0.0f};
        vertices[2] = { 5 + x,  5 + y, 0.0f};
        vertices[3] = {-5 + x,  5 + y, 0.0f};

        vertices[0].uvs = { 0.0f, 0.0f};
        vertices[1].uvs = { 1.0f, 0.0f};
        vertices[2].uvs = { 1.0f, 1.0f};
        vertices[3].uvs = { 0.0f, 1.0f};

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

        GLuint indices[6] = {0, 1, 3, 1, 2, 3};

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uvs));

        glBindVertexArray(0);

        Image image = BitmapReader::ReadFile("resources/sprites/button.bmp");
        sprite = Sprite::Create(&image);

        delete[] image.pixels;
    }

    void Draw(){
        sprite->Load();

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        sprite->UnLoad();

    }

    bool IsMouseOver(float mouseX, float mouseY) const {
        return (vertices[0].pos.x <= mouseX && mouseX <= vertices[1].pos.x) &&
                (vertices[0].pos.y <= mouseY && mouseY <= vertices[2].pos.y);
    }

    void OnClick() const {
        if (onClick)
            onClick();
    }

    ~Button() {
        delete sprite;

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
};

void Render();

GLint modelLocation;
std::vector<Component*> components;

int main(){

    GraphicConfig::vsync = true;
    GraphicConfig::zbuffer = true;
    GraphicConfig::windowHeight = 1000;
    GraphicConfig::windowWidth = 1000;
    GraphicConfig::windowTitle = "Translations";

    // Create context
    WindowContext context;

    // Create system
    GraphicSystem graphic( &context );

    MouseButtonMonitor monitor( &context );
    //KeyboardMonitor keyMonitor( &context );

    // Initialize affine stack
    TransformStack::Push();
    TransformStack::Ortho(0, 1000, 0, 1000, -100.0f, 100.0f);

    // Create shader
    GLShader mainShader;
    mainShader.LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
    mainShader.LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
    mainShader.Build();

    // Locate model in shader
    modelLocation = mainShader.GetUniformLocation("model");

    // Create objects

    Component * button = new Component(500, 500);

    components.emplace_back(button);

    // Enable shader
    mainShader.Use();

    while( !context.ShouldClose() ){

        Render();

        graphic.Run();
    }

    mainShader.Dispose();

    for(Component * Component : components)
        delete Component;

    return 0;
}


void Render(){

    static std::chrono::high_resolution_clock::time_point last;
    static int frames;

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    frames++;

    float delta = std::chrono::duration_cast< std::chrono::duration<float> >(now - last).count();

    if( delta >= 1.0f){
        printf("FPS : %d\r", frames);
        fflush(stdout);
        frames = 0;
        last = now;
    }

    static float angle;
    angle += 1.0f;

    TransformStack::Push();
    TransformStack::Translate(500.0f, 500.0f ,0.0f);
    TransformStack::Rotate(180.0f, 0.0f, 0.0f, 1.0f);
    TransformStack::Rotate(angle, 0.0f, 0.0f, 1.0f);
    TransformStack::Scale(20.0f, 20.0f, 1.0f);
    TransformStack::Translate(-500.0f, -500.0f ,0.0f);

    Matrix model = TransformStack::Top();
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.Data());

    for (int i = 0; i < components.size(); i++) {
        Component * component = components[i];
        component->Draw();
    }

    TransformStack::Pop();
}
