#include "Application.h"

#include "CanvasElement.h"

Camera * mainCamera;
KeyboardMonitor * monitor;
MouseButtonMonitor * mouseMonitor;

class SolarSystem : public System{

    Scene * scene;
    SceneObject * planet;
    TextElement * text;

    void Execute() override{

        static float angle, theta;

        Vector3& planetPosition = planet->transform.position;

        planetPosition.x = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI) + GraphicConfig::windowWidth*0.5f;
        planetPosition.y = 100.0f * sin(theta * 2.0f * M_PI) + GraphicConfig::windowHeight*0.5f;
        planetPosition.z = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI);

        float ry = theta;
        float rz = 2.0f * angle;

        planet->transform.rotation = Quaternion::ToQuaternion(Vector3(angle,ry,rz));


        angle += 0.00001f;
        theta += 0.0001f * angle;

    }

    void Share() override{

    }

public:

    SolarSystem(Scene * scene){
        this->scene = scene;

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
    monitor = &app.GetKeyboardInputMonitor();
    mouseMonitor = &app.GetMouseInputMonitor();

    SolarSystem * solar = new SolarSystem(&scene);

    app.RegisterSystem(solar);
    app.LoadScene(scene);

    app.Loop();

    return 0;
}
