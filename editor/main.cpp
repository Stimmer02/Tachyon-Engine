#include "Application.h"

#include "CanvasElement.h"

Camera * mainCamera;

class SolarSystem : public System{

    Scene * scene;
    SceneObject * planet;
    TextElement * text;
    Input * instance;

    void Execute() override{

        static float angle, theta, lastX, lastY;


        Vector3& planetPosition = planet->transform.position;
        Timer& timer = Timer::GetInstance();

        planetPosition.x = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI) + GraphicConfig::windowWidth*0.5f;
        planetPosition.y = 100.0f * sin(theta * 2.0f * M_PI) + GraphicConfig::windowHeight*0.5f;
        planetPosition.z = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI);

        planet->transform.rotation = Quaternion::ToQuaternion(Vector3(angle, theta, 2.0f * angle));

        Vector3& position = mainCamera->position;

        EventType wS = instance->GetKeyState(GLFW_KEY_W);
        EventType sS = instance->GetKeyState(GLFW_KEY_S);
        EventType aS = instance->GetKeyState(GLFW_KEY_A);
        EventType dS = instance->GetKeyState(GLFW_KEY_D);

        printf("States : %d\t%d\t%d\t%d\n", wS, sS, aS, dS);

        if ( wS == ONHOLD){
            position.y += timer.GetDeltaTime() ;
        }

        if ( sS == ONHOLD){
            position.y -= timer.GetDeltaTime();
        }

        if ( aS == ONHOLD){
            position.x -= timer.GetDeltaTime();
        }

        if ( dS == ONHOLD){
            position.x += timer.GetDeltaTime();
        }



        angle += 0.00001f;
        theta += 0.0001f * angle;

    }

    void Share() override{

    }

public:

    SolarSystem(Scene * scene){
        this->scene = scene;
        this->instance = &Input::GetInstance();

        planet = scene->CreateEntity();

        scene->AddEntityToScene(planet);


        Mesh * m = planet->AddAttribute<Mesh>();
        planet->AddAttribute<Sprite>("resources/sprites/heart.bmp");
        m->GenCube(100.0f, 100.0f, 100.0f);


        if( GraphicConfig::useOrthographicProjection )
            text = new TextElement("Orthographic");
        else
            text = new TextElement("Perspective");

        text->transform.position.x = 50.0f;
        text->transform.position.y = 50.0f;

        text->SetFontSize(12.0f);

        scene->AddGUIToScene(text);

    }

};

int main(){

    GraphicConfig::vsync = false;
    GraphicConfig::zbuffer = true;
    GraphicConfig::windowHeight = 600;
    GraphicConfig::windowWidth = 800;
    GraphicConfig::windowTitle = "Application";

    // GraphicConfig::useOrthographicProjection = false;
    ApplicationConfig::internalGUIInteraction = false;

    Application app;

    Scene scene;

    mainCamera = &app.GetMainCamera();
    SolarSystem * solar = new SolarSystem(&scene);

    app.RegisterSystem(solar);
    app.LoadScene(scene);

    app.Loop();

    return 0;
}
