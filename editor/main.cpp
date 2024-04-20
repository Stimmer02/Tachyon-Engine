#include "GLShader.h"
#include "TransformStack.h"
#include "Vertex.h"

#include "BitmapReader.h"
#include "Sprite.h"

#include "Application.h"

#include <array>

#include "Attribute.h"

class GLComponent{
private:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;


    Sprite * sprite;

    std::array<vertex, 4> vertices;

public:

    float x, y;

    GLComponent(float x, float y){

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

    ~GLComponent(){

        delete sprite;

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

};

int main(){

    GraphicConfig::vsync = true;
    GraphicConfig::zbuffer = false;
    GraphicConfig::windowHeight = 1000;
    GraphicConfig::windowWidth = 1000;
    GraphicConfig::windowTitle = "Application";

    Application app;

    GLShader mainShader;
    mainShader.LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
    mainShader.LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
    mainShader.Build();

    Scene scene;

    SceneObject * object = scene.CreateEntity();
    // object->GetTransform().position = {GraphicConfig::windowWidth/2, GraphicConfig::windowHeight/2, 0};

    app.LoadScene(scene);

    mainShader.Use();

    // Initialize affine stack
    TransformStack::Push();
    TransformStack::Ortho(0, 1000, 0, 1000, -100.0f, 100.0f);

    // Execute main loop;
    app.Loop();

    mainShader.Dispose();

    return 0;
}
