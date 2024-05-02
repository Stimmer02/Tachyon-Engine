#include "Application.h"

Camera * mainCamera;
KeyboardMonitor * monitor;

class SolarSystem : public System{

    Scene * scene;
    SceneObject * sun, * mercury, * venus, * venusMoon;
    Sprite * sunSprite;
    Timer * timer;

    void Execute() override{

        static float angle;

        float deltaTime = timer->GetDeltaFrame();

        mercury->transform.position.x = 2.0f * cos( 0.5f * angle );
        mercury->transform.position.y = 2.0f * sin( 0.5f * angle );

        venus->transform.position.x = 3.0f * cos(2.0f * angle );
        venus->transform.position.y = 3.0f * sin(2.0f * angle );

        sun->transform.rotation = Quaternion::ToQuaternion({angle * 0.5f, -angle, angle});

        venusMoon->transform.position.x = 2.0f;
        venusMoon->transform.position.y = 0.0f;

        EventInfo info = monitor->GetButtonState(GLFW_KEY_W);

        if( info.type == ONHOLD )
            mainCamera->MoveBy(Vector3(0.0f, 0.0f, -1.0f), deltaTime);

        info = monitor->GetButtonState(GLFW_KEY_S);

        if( info.type == ONHOLD )
            mainCamera->MoveBy(Vector3(0.0f, 0.0f, 1.0f), deltaTime);

        info = monitor->GetButtonState(GLFW_KEY_A);

        if( info.type == ONHOLD )
            mainCamera->MoveBy(Vector3(-1.0f, 0.0f, 0.0f), deltaTime);

        info = monitor->GetButtonState(GLFW_KEY_D);

        if( info.type == ONHOLD )
            mainCamera->MoveBy(Vector3(1.0f, 0.0f, 0.0f), deltaTime);

        info = monitor->GetButtonState(GLFW_KEY_SPACE);

        if( info.type == ONHOLD )
            mainCamera->MoveBy(Vector3(0.0f, 1.0f, 0.0f), deltaTime);

        info = monitor->GetButtonState(GLFW_KEY_LEFT_SHIFT);

        if( info.type == ONHOLD )
            mainCamera->MoveBy(Vector3(0.0f, -1.0f, 0.0f), deltaTime);

        angle *= (angle < 360.0f);
        angle += 1.0f * deltaTime;

        if( timer->GetAccumulatedTime() >= 1.0f){
            sunSprite->NextFrame();
        }

    }

    void Share() override{

    }

public:

    SolarSystem(Scene * scene){
        this->scene = scene;
        this->timer = &Timer::GetInstance();

        sun = scene->CreateEntity();
        scene->AddEntityToScene(sun);

        mercury = scene->CreateEntity();
        sun->AddChildren(mercury);

        venus = scene->CreateEntity();
        sun->AddChildren(venus);

        venusMoon = scene->CreateEntity();
        venus->AddChildren(venusMoon);

        sun->transform.position = Vector3(400, 300);
        sun->transform.scale = Vector3(50.0f, 50.0f, 50.0f);
        mercury->transform.scale = Vector3(0.5f, 0.5f, 0.5f);
        venus->transform.scale = Vector3(0.3f, 0.3f, 0.3f);
        venusMoon->transform.scale = Vector3(0.3f, 0.3f, 0.3f);

        Mesh * sunMesh = sun->AddAttribute<Mesh>();
        Mesh * mercuryMesh = mercury->AddAttribute<Mesh>();
        Mesh * venusMesh = venus->AddAttribute<Mesh>();
        Mesh * venusMoonMesh = venusMoon->AddAttribute<Mesh>();

        sunMesh->GenSphere(1.0f, 32, 32);
        mercuryMesh->GenSphere(1.0f, 10, 10);
        venusMesh->GenSphere(1.0f, 10, 10);
        venusMoonMesh->GenSphere(1.0f, 10, 10);

        Color colors[] = { {200, 128, 0}, {127, 127, 127}, {127, 0, 127}, {80, 80, 80} };


        sunSprite = sun->AddAttribute<Sprite>("resources/sprites/test.bmp");
        sunSprite->Push("resources/sprites/slime.bmp");
        Sprite * mercurySprite = mercury->AddAttribute<Sprite>(colors + 1, 1, 1);
        Sprite * venusSprite = venus->AddAttribute<Sprite>(colors + 2, 1, 1);
        Sprite * venusMoonSprite = venusMoon->AddAttribute<Sprite>(colors + 3, 1, 1);

    }


};

int main(){

    srand(time(nullptr));

    GraphicConfig::vsync = false;
    GraphicConfig::zbuffer = true;
    GraphicConfig::windowHeight = 600;
    GraphicConfig::windowWidth = 800;
    GraphicConfig::windowTitle = "Application";

    Application app;

    Scene scene;

    mainCamera = &app.GetMainCamera();
    monitor = &app.GetKeyboardInputMonitor();

    SolarSystem * solar = new SolarSystem(&scene);

    app.RegisterSystem(solar);
    app.LoadScene(scene);

    app.Loop();

    return 0;
}
