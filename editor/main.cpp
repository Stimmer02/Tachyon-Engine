#include "Application.h"


Camera * mainCamera;

class SolarSystem : public System{

    Scene * scene;
    SceneObject * planet;
    TextElement * text;
    Input * instance;
    Mesh * renderer;

    void Execute() override{

        static float angle;

        Timer& timer = Timer::GetInstance();

        planet->transform.rotation = Quaternion::ToQuaternion(Vector3(angle, angle * 0.1f, angle * 0.5f));

        angle += timer.GetDeltaFrame();

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

    }

    void Share() override{

    }

public:

    SolarSystem(Scene * scene){
        this->scene = scene;
        this->instance = &Input::GetInstance();

        planet = scene->CreateEntity();

        scene->AddEntityToScene(planet);

        planet->transform.position.x = GraphicConfig::windowWidth*0.5f;
        planet->transform.position.y = GraphicConfig::windowHeight*0.5f;

        Sprite * s = planet->AddAttribute<Sprite>("resources/sprites/heart.bmp");
        s->SetTextureAttrib(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        s->SetTextureAttrib(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        renderer = planet->AddAttribute<Mesh>();
        renderer->GenTorus(100.0f, 40.0f);
        // canvas = new CanvasElement(GraphicConfig::windowWidth*0.5f - 150, GraphicConfig::windowHeight*0.5f - 150, 300, 300);
        // canvas->ClearCanvas({255,255,255});
        // canvas->SetCellSize(5);
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

    // GraphicConfig::useOrthographicProjection = false;
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
