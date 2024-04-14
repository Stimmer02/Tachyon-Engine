#include "GLShader.h"
#include "TransformStack.h"
#include "Vertex.h"

#include "BitmapReader.h"
#include "Sprite.h"

#include "Application.h"

#include <array>

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

GLComponent * component;
KeyboardMonitor * ptr;

void Render();

int main(){

    srand( time(0) );

    GraphicConfig::vsync = true;
    GraphicConfig::zbuffer = false;
    GraphicConfig::windowHeight = 1000;
    GraphicConfig::windowWidth = 1000;
    GraphicConfig::windowTitle = "Application";

    Application app;
    app.SetRenderFunc(Render);

    MouseButtonMonitor monitor = app.GetMouseInputMonitor();
    KeyboardMonitor kmonitor = app.GerKeyboardInputMonitor();

    ptr = & kmonitor;

    // Initialize affine stack
    TransformStack::Push();
    TransformStack::Ortho(0, 1000, 0, 1000, -100.0f, 100.0f);

    // Create shader
    GLShader mainShader;
    mainShader.LinkShader("./resources/shaders/vertexShader.vert", GL_VERTEX_SHADER);
    mainShader.LinkShader("./resources/shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
    mainShader.Build();

    // Create objects
    component = new GLComponent(500, 500);

    // Enable shader
    mainShader.Use();

    // Execute main loop;
    app.Loop();

    // Disable shader
    mainShader.Dispose();

    return 0;
}

void Render(){

    static float angle;
    static int mode;

    const float speed = 100.0f;

    Timer& timer = Timer::GetInstance();

    TransformStack::Push();
    TransformStack::Translate(component->x, component->y, 0.0f);
    TransformStack::Scale(10.0f, 10.0f, 1.0f);

    if(mode == 0){
        TransformStack::Rotate(angle, 0.0f, 0.0f, 1.0f);
        angle *= ( angle < 360.0f);
        angle += 0.1f * timer.GetDeltaFrame() * speed;
    }else if(mode == 1){
        TransformStack::Rotate(180.0f, 0.0f, 0.0f, 1.0f);
    }else{
        float t = glfwGetTime();
        float x = cos(t);
        float y = sin(t);

        TransformStack::Scale(x, y, 1);
    }

    TransformStack::Translate(-component->x, -component->y, 0.0f);

    Matrix model = TransformStack::Top();
    GLuint modelLocation = currentShader->GetUniformLocation("model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.Data());
    TransformStack::Pop();

    component->Draw();

    EventInfo infoR = ptr->GetButtonState(GLFW_KEY_R);
    EventInfo infoT = ptr->GetButtonState(GLFW_KEY_T);
    EventInfo infoY = ptr->GetButtonState(GLFW_KEY_Y);

    if( infoR.type == ONTRIGGER ){
        mode = 0;
        angle = 180.0f;
    }

    if( infoT.type == ONTRIGGER ){
        mode = 1;
    }

    if( infoY.type == ONTRIGGER ){
        mode = 2;
    }

    if( timer.GetAccumulatedTime() >= 1.0f){
        fprintf(stdout, "FPS : %d\r", timer.GetFrameCount());
        fflush(stdout);
    }

    static bool flipX, flipY;

    if( component->x <= 0.0f || component->x >= GraphicConfig::windowWidth)
        flipX^=true;

    if( component->y <= 0.0f || component->y >= GraphicConfig::windowHeight)
        flipY^=true;


    component->x += ( (2.0f * (flipX) - 1.0f) + (0.5f * rand()/(float)RAND_MAX - 0.25f) ) * timer.GetDeltaFrame() * speed;
    component->y += ( (2.0f * (flipY) - 1.0f) + (0.5f * rand()/(float)RAND_MAX - 0.25f) ) * timer.GetDeltaFrame() * speed;

}
