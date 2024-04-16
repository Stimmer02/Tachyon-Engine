#include "GLShader.h"
#include "TransformStack.h"
#include "Vertex.h"

#include "BitmapReader.h"
#include "Sprite.h"

#include "Application.h"

#include <array>

#define SOURCES {"resources/sprites/13.bmp", "resources/sprites/heart.bmp", "resources/sprites/slime.bmp", "resources/sprites/test.bmp"}
#define UP {0.0f, 0.0f, 1.0f}

position CrossProduct(const position & u, const position & v){

    position p;

    p.x = u.y*u.z - v.y*u.z;
    p.y = u.z*v.x - u.x*v.z;
    p.z = u.x*v.y - u.y*v.x;

    return p;

}

class GLComponent{
protected:

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei indexCount;

    Sprite * sprite;

public:

    float x, y;
    float vx, vy;
    float angle;
    bool flipX;
    bool flipY;

    GLComponent(float x, float y){

        this->x = x;
        this->y = y;

        int idx = rand()%4;

        const char * resources[4] = SOURCES;

        Image image = BitmapReader::ReadFile(resources[idx]);
        sprite = Sprite::Create(&image);
        delete[] image.pixels;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    virtual void Fill() = 0;

    void Draw() const{

        GLuint offsetLocation = currentShader->GetUniformLocation("offset");
        glUniform3f(offsetLocation, x, y, 0.0f);

        sprite->Load();

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        sprite->UnLoad();

    }

    virtual ~GLComponent(){

        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

};

class Quad : public GLComponent{

private:

    std::array<vertex, 4> vertices;

public:

    Quad(float x, float y) : GLComponent(x, y){
        Fill();
    }

    ~Quad(){

    }

    void Fill() override{
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

        indexCount = 6;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uvs));

        glBindVertexArray(0);

    }

};

class Sphere : public GLComponent {
private:
    int sectors;
    int stacks;
    float radius = 20.0f;

    std::vector<vertex> vertices;

public:
    Sphere(float x, float y, int sectors = 18, int stacks = 9) : GLComponent(x, y), sectors(sectors), stacks(stacks) {
        Fill();
    }

    ~Sphere(){

    }

    void Fill() override {

        std::vector<GLuint> indices;

        const float pi = 3.1415926535f;

        for (int i = 0; i <= stacks; ++i) {

            float stackAngle = pi * (float)i / (float)stacks;

            float y = radius * cos(stackAngle);
            float r = radius * sin(stackAngle);

            for (int j = 0; j <= sectors; ++j) {
                float sectorAngle = 2.0f * pi * (float)j / (float)sectors;
                float x = r * cos(sectorAngle);
                float z = r * sin(sectorAngle);

                vertex vt;
                vt.pos = {x, y, z};

                float u = (float)j / (float)sectors;
                float v = (float)i / (float)stacks;

                vt.uvs = {u, v};
                vertices.push_back(vt);
            }

        }

        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;

            for (int j = 0; j < sectors; ++j) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }

                ++k1;
                ++k2;
            }
        }

        indexCount = indices.size();

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uvs));

        glBindVertexArray(0);

    }
};

std::vector<GLComponent*> components;
KeyboardMonitor * ptr;

void Render();

int main(){

    srand( time(0) );

    GraphicConfig::vsync = false;
    GraphicConfig::zbuffer = true;
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
    for(int i = 0; i < 50; i++){

        float angle = (2.0f * rand()/(float)RAND_MAX - 1.0f) * 360.0f;

        float x = GraphicConfig::windowWidth/2.0f + 10 * cos(angle);
        float y = GraphicConfig::windowHeight/2.0f + 10 * sin(angle);

        GLComponent * component = new Sphere(x, y);

        component->vx = cos(angle);
        component->vy = sin(angle);
        component->angle = 0.0f;

        components.emplace_back(component);
    }

    // Enable shader
    mainShader.Use();

    // Execute main loop;
    app.Loop();

    // Disable shader
    mainShader.Dispose();

    // Clear memory
    for(int i = 0; i < components.size(); i++){
        delete components[i];
    }

    return 0;
}

void Render(){

    static int mode = 0;
    static bool isRunning = true;
    const float speed = 20.0f;

    Timer& timer = Timer::GetInstance();

    Matrix model = TransformStack::Top();
    GLuint modelLocation = currentShader->GetUniformLocation("model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.Data());

    const position wu = UP;

    for(GLComponent * component : components){
        TransformStack::Push();
        TransformStack::Translate(component->x, component->y, 0.0f);

        float deltaX = (2.0f * component->flipX - 1.0f) * component->vx;
        float deltaY = (2.0f * component->flipY - 1.0f) * component->vy;

        position dir = {deltaX, deltaY, 0.0f};
        position axis = CrossProduct(wu, dir);

        if (mode == 0) {
            TransformStack::Rotate(component->angle, axis.x, axis.y, axis.z);
        } else if (mode == 1) {
            TransformStack::Rotate(180.0f, 0.0f, 0.0f, 1.0f);
        } else {
            float t = glfwGetTime() * isRunning;
            float scaleX = cosf(t) * 2.0f + 2.0f;
            float scaleY = sinf(t) * 2.0f + 2.0f;
            TransformStack::Scale(scaleX, scaleY, 1.0f);
        }

        TransformStack::Translate(-component->x, -component->y, 0.0f);

        Matrix model = TransformStack::Top();
        GLuint modelLocation = currentShader->GetUniformLocation("model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.Data());

        TransformStack::Pop();

        component->Draw();
    }

    EventInfo infoR = ptr->GetButtonState(GLFW_KEY_R);
    EventInfo infoT = ptr->GetButtonState(GLFW_KEY_T);
    EventInfo infoY = ptr->GetButtonState(GLFW_KEY_Y);
    EventInfo infoS = ptr->GetButtonState(GLFW_KEY_SPACE);
    EventInfo infoH = ptr->GetButtonState(GLFW_KEY_H);

    if (infoR.type == ONTRIGGER) {
        mode = 0;
    }else if (infoT.type == ONTRIGGER) {
        mode = 1;
    }else if (infoY.type == ONTRIGGER) {
        mode = 2;
    }else if(infoH.type == ONTRIGGER){

        static bool isMeshVisible = false;

        if( isMeshVisible ){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        isMeshVisible^=true;

    }else if( infoS.type == ONTRIGGER){
        isRunning ^= true;
    }


    if (timer.GetAccumulatedTime() >= 1.0f) {
        fprintf(stdout, "FPS : %d\r", timer.GetFrameCount());
        fflush(stdout);
    }

    if( !isRunning )
        return;

    for(GLComponent * component : components){

        float deltaX = (2.0f * component->flipX - 1.0f) * component->vx;
        float deltaY = (2.0f * component->flipY - 1.0f) * component->vy;

        component->x += deltaX * timer.GetDeltaFrame() * speed;
        component->y += deltaY * timer.GetDeltaFrame() * speed;

        float len = sqrt(deltaX * deltaX + deltaY * deltaY);

        component->angle += len/20.0f;

        if (component->x <= 20.0f || component->x >= (GraphicConfig::windowWidth - 20.0f) ){
            component->flipX ^= true;
            component->x = fmax( 20.0f, fmin(component->x, (GraphicConfig::windowWidth - 20.0f) ));
        }

        if (component->y <= 20.0f || component->y >= (GraphicConfig::windowHeight - 20.0f) ){
            component->flipY ^= true;
            component->y = fmax( 20.0f, fmin(component->y, (GraphicConfig::windowHeight - 20.0f) ));
        }

    }

}
