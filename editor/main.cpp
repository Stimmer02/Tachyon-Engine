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
        static float rx,ry,rz;


        Vector3& planetPosition = planet->transform.position;

        planetPosition.x = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI) + GraphicConfig::windowWidth*0.5f;
        planetPosition.y = 100.0f * sin(theta * 2.0f * M_PI) + GraphicConfig::windowHeight*0.5f;
        planetPosition.z = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI);


        if ( instance->GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == ONHOLD ){


            Vector3 position = instance->GetMousePosition();
            float dx = (position.x - lastX);
            float dy = (lastY - position.y);
            lastX = position.x ;
            lastY = position.y ;

            float magnitude = std::sqrt(dx*dx +dy*dy);

            if( std::fabs(magnitude) < 1e-6f)
                magnitude = 1.0f;

            dx/=magnitude;
            dy/=magnitude;

            rx += dx * M_PI * 0.05f;
            ry += dy * M_PI * 0.05f;

            planet->transform.rotation = Quaternion::ToQuaternion(Vector3(ry,rx,rz));

        }

        if ( instance->GetKeyState(GLFW_KEY_R) == ONTRIGGER){
            rx = 0.0f;
            ry = 0.0f;
            rz = 0.0f;
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
