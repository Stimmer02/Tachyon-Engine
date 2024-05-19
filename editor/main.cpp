#include "Application.h"

class SolarSystem : public System{

    Scene * scene;
    Camera * mainCamera;
    Timer * timer;

    SceneObject * planet;
    TextElement * text;
    Input * instance;
    Mesh * renderer;

    void Execute() override{

        static float angle;

        planet->transform.rotation = Quaternion::ToQuaternion(Vector3(angle, angle * 0.1f, angle * 0.5f));

        angle += timer->GetDeltaFrame();

        if( instance->GetKeyState(GLFW_KEY_A) == ONHOLD ){
            mainCamera->MoveBy(Vector3(-1.0,0.0,0.0), timer->GetDeltaTime());
        }

        if( instance->GetKeyState(GLFW_KEY_D) == ONHOLD ){
            mainCamera->MoveBy(Vector3(1.0,0.0,0.0), timer->GetDeltaTime());
        }

        if( instance->GetKeyState(GLFW_KEY_W) == ONHOLD ){
            mainCamera->MoveBy(Vector3(0.0,1.0,0.0), timer->GetDeltaTime());
        }

        if( instance->GetKeyState(GLFW_KEY_S) == ONHOLD ){
            mainCamera->MoveBy(Vector3(0.0,-1.0,0.0), timer->GetDeltaTime());
        }

    }

public:

    SolarSystem(){

        this->instance = &Input::GetInstance();

    }

    void OnLoad() override{

        planet = scene->CreateEntity();

        planet->transform.position.x = GraphicConfig::windowWidth * 0.5f;
        planet->transform.position.y = GraphicConfig::windowHeight * 0.5f;


        Sprite * s = planet->AddAttribute<Sprite>("resources/sprites/heart.bmp");
        s->SetTextureAttrib(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        s->SetTextureAttrib(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::vector<Vector3> vertices;

        const int numVert = 30;
        float radius = 30.0f;
        float height = 0.0f;

        for(int i = 0; i < numVert; i++){

            const float angle = i * M_PI/180.0f;

            Vector3 v = Vector3(radius * cos(angle),radius * sin(angle), height);

            vertices.push_back(v);

            radius += 0.1f;
            height += 1.0f;

        }

        renderer = planet->AddAttribute<Mesh>();
        // renderer->SetVertices(vertices.data(), vertices.size());
        renderer->GenTorus(100.0f, 50.0f);

        if( GraphicConfig::useOrthographicProjection )
            text = new TextElement("Orthographic");
        else
            text = new TextElement("Perspective");

        text->transform.position.x = 50.0f;
        text->transform.position.y = 50.0f;

        text->SetFontSize(12.0f);

        scene->AddEntityToScene(planet);
        scene->AddGUIToScene(text);

    }

    void Share(SharedNameResolver * resourceManager) override{
        this->timer = (Timer*)resourceManager->Find("timer");
        this->scene = (Scene*)resourceManager->Find("scene");
        this->mainCamera = (Camera*)resourceManager->Find("camera");
    }

};

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    SolarSystem * solar = new SolarSystem();
    app.RegisterSystem(solar);

    app.Loop();

    return 0;
}
