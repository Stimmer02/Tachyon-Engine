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

    float x, y;

    Component(float x, float y){

        this->x = x;
        this->y = y;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        vertices[0] = {-10, -10, 0.0f};
        vertices[1] = { 10, -10, 0.0f};
        vertices[2] = { 10,  10, 0.0f};
        vertices[3] = {-10,  10, 0.0f};

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

        GLuint offsetLocation = currentShader->GetUniformLocation("offset");
        glUniform3f(offsetLocation, x, y, 0.0f);

        sprite->Load();

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        sprite->UnLoad();

    }

    bool IsMouseOver(float mouseX, float mouseY) const {
        return (vertices[0].pos.x + x <= mouseX && mouseX <= vertices[1].pos.x + x) &&
                (vertices[0].pos.y + y <= mouseY && mouseY <= vertices[2].pos.y + y);
    }

    ~Component(){

        delete sprite;

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

};

void Render();

std::vector<Component*> components;

int main(){

    GraphicConfig::vsync = false;
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


    // Create objects

    Component * button = new Component(500, 500);

    components.emplace_back(button);

    // Enable shader
    mainShader.Use();

    bool isHolding = false;

    while( !context.ShouldClose() ){

        EventInfo info = monitor.Query(GLFW_MOUSE_BUTTON_LEFT);

        if( info.type == ONTRIGGER)
            isHolding = true;

        if( isHolding && button->IsMouseOver(info.x, info.y)){
            button->x = info.x;
            button->y = info.y;
        }

        if( info.type == ONRELEASE)
            isHolding = false;

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

    Matrix model = TransformStack::Top();
    GLuint modelLocation = currentShader->GetUniformLocation("model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.Data());

    for (int i = 0; i < components.size(); i++) {
        Component * component = components[i];
        component->Draw();
    }
}
