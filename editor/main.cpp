#include "Application.h"

#include "CanvasElement.h"

Camera * mainCamera;

class SolarSystem : public System{

    Scene * scene;
    SceneObject * planet;
    TextElement * text;
    Input * instance;
    CanvasElement * canvas;

    void Execute() override{

        static float angle, theta, lastX, lastY;
        static Color color;

        Vector3& planetPosition = planet->transform.position;
        Timer& timer = Timer::GetInstance();

        planetPosition.x = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI) + GraphicConfig::windowWidth*0.5f;
        planetPosition.y = 100.0f * sin(theta * 2.0f * M_PI) + GraphicConfig::windowHeight*0.5f;
        planetPosition.z = 100.0f * cos(angle * 2.0f * M_PI) * cos(theta * 2.0f * M_PI);

        planet->transform.rotation = Quaternion::ToQuaternion(Vector3(angle, theta, 2.0f * angle));

        if( instance->GetKeyState(GLFW_KEY_0) == ONTRIGGER ){
            color = {};
        }

        if( instance->GetKeyState(GLFW_KEY_1) == ONTRIGGER ){
            color = {255, 0, 0};
        }

        if( instance->GetKeyState(GLFW_KEY_2) == ONTRIGGER ){
            color = {0, 255, 0};
        }

        if( instance->GetKeyState(GLFW_KEY_3) == ONTRIGGER ){
            color = {0, 0, 255};
        }

        if( instance->GetKeyState(GLFW_KEY_4) == ONTRIGGER ){
            color = {255, 255, 255};
        }

        if( instance->GetButtonState(GLFW_MOUSE_BUTTON_LEFT) == ONHOLD ){
            Vector3 mousePos = instance->GetMousePosition();
            if(canvas->isInBound(mousePos.x, mousePos.y))
                canvas->SetPixel(mousePos.x, mousePos.y, color);

        }


        if( instance->GetKeyState(GLFW_KEY_A) == ONHOLD ){
            mainCamera->position.x -= timer.GetDeltaTime();
        }

        if( instance->GetKeyState(GLFW_KEY_D) == ONHOLD ){
            mainCamera->position.x += timer.GetDeltaTime();
        }

        if( instance->GetKeyState(GLFW_KEY_W) == ONHOLD ){
            mainCamera->position.z -= timer.GetDeltaTime();
        }

        if( instance->GetKeyState(GLFW_KEY_S) == ONHOLD ){
            mainCamera->position.z += timer.GetDeltaTime();
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

        mainCamera->position.x = GraphicConfig::windowWidth*0.5f;
        mainCamera->position.y =  GraphicConfig::windowHeight*0.5f;
        mainCamera->position.z =  GraphicConfig::windowHeight*0.5f;

        Mesh * m = planet->AddAttribute<Mesh>();
        planet->AddAttribute<Sprite>("resources/sprites/heart.bmp");
        m->GenTorus(100.0f, 40.0f);

        canvas = new CanvasElement(GraphicConfig::windowWidth*0.5f - 150, GraphicConfig::windowHeight*0.5f - 150, 300, 300);
        canvas->ClearCanvas({255,255,255});
        canvas->SetCellSize(5);

        // scene->AddGUIToScene(canvas);

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

    GraphicConfig::useOrthographicProjection = false;
    // ApplicationConfig::internalGUIInteraction = false;

    Application app;

    Scene scene;

    mainCamera = &app.GetMainCamera();
    SolarSystem * solar = new SolarSystem(&scene);

    app.RegisterSystem(solar);
    app.LoadScene(scene);

    app.Loop();

    return 0;
}
