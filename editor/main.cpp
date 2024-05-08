#include "Application.h"

#include "CanvasElement.h"

Camera * mainCamera;
KeyboardMonitor * monitor;
MouseButtonMonitor * mouseMonitor;

class SolarSystem : public System{

    Scene * scene;
    SceneObject * sun, * mercury, * venus, * venusMoon;
    Sprite * sunSprite;
    Timer * timer;

    ButtonElement * button;
    CanvasElement * canvas, * canvasPrime, * canvas1, * canvas2, * canvas3, * canvas4, * canvas5, * canvas6, * canvas7, * canvas8, * canvas9, * canvas10;

    void Execute() override{

        // static float angle;

        // float deltaTime = timer->GetDeltaFrame();

        // mercury->transform.position.x = 2.0f * cos( 0.5f * angle );
        // mercury->transform.position.y = 2.0f * sin( 0.5f * angle );

        // venus->transform.position.x = 3.0f * cos(2.0f * angle );
        // venus->transform.position.y = 3.0f * sin(2.0f * angle );

        // sun->transform.rotation = Quaternion::ToQuaternion({angle * 0.5f, -angle, angle});

        // venusMoon->transform.position.x = 2.0f;
        // venusMoon->transform.position.y = 0.0f;

        // EventInfo info = monitor->GetButtonState(GLFW_KEY_W);

        // if( info.type == ONHOLD )
        //     mainCamera->MoveBy(Vector3(0.0f, 0.0f, -1.0f), deltaTime);

        // info = monitor->GetButtonState(GLFW_KEY_S);

        // if( info.type == ONHOLD )
        //     mainCamera->MoveBy(Vector3(0.0f, 0.0f, 1.0f), deltaTime);

        // info = monitor->GetButtonState(GLFW_KEY_A);

        // if( info.type == ONHOLD )
        //     mainCamera->MoveBy(Vector3(-1.0f, 0.0f, 0.0f), deltaTime);

        // info = monitor->GetButtonState(GLFW_KEY_D);

        // if( info.type == ONHOLD )
        //     mainCamera->MoveBy(Vector3(1.0f, 0.0f, 0.0f), deltaTime);

        // info = monitor->GetButtonState(GLFW_KEY_SPACE);

        // if( info.type == ONHOLD )
        //     mainCamera->MoveBy(Vector3(0.0f, 1.0f, 0.0f), deltaTime);

        // info = monitor->GetButtonState(GLFW_KEY_LEFT_SHIFT);

        // if( info.type == ONHOLD )
        //     mainCamera->MoveBy(Vector3(0.0f, -1.0f, 0.0f), deltaTime);

        // info = monitor->GetButtonState(GLFW_KEY_Q);

        // if( info.type == ONHOLD )
        //     mainCamera->RotateBy(-1.0f, 0.0f);

        // info = monitor->GetButtonState(GLFW_KEY_E);

        // if( info.type == ONHOLD )
        //     mainCamera->RotateBy(1.0f, 0.0f);


        // angle *= (angle < 360.0f);
        // angle += 1.0f * deltaTime;

        // EventInfo info = mouseMonitor->GeyKeyState(GLFW_MOUSE_BUTTON_LEFT);

        // if( info.type == ONTRIGGER ){

        //     if( canvas->isInBound(info.x, info.y) ){
        //         canvas->SetPixel(info.x, info.y, {0,0,0});
        //     }
        // }

    }

    void Share() override{

    }

public:

    SolarSystem(Scene * scene){
        this->scene = scene;
        this->timer = &Timer::GetInstance();

        sun = scene->CreateEntity();
        // scene->AddEntityToScene(sun);

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

        sunMesh->GenSphere(1.0f, 8, 8);
        mercuryMesh->GenSphere(1.0f, 10, 10);
        venusMesh->GenSphere(1.0f, 10, 10);
        venusMoonMesh->GenSphere(1.0f, 10, 10);

        Color colors[] = { {200, 128, 0}, {127, 127, 127}, {127, 0, 127}, {80, 80, 80} };


        sunSprite = sun->AddAttribute<Sprite>("resources/sprites/test.bmp");
        sunSprite->Push("resources/sprites/slime.bmp");
        Sprite * mercurySprite = mercury->AddAttribute<Sprite>(colors + 1, 1, 1);
        Sprite * venusSprite = venus->AddAttribute<Sprite>(colors + 2, 1, 1);
        Sprite * venusMoonSprite = venusMoon->AddAttribute<Sprite>(colors + 3, 1, 1);

        // canvas = new CanvasElement(400, 300, 100, 100);
        // scene->AddGUIToScene(canvas);
        
        
        // canvas1 = new CanvasElement(0, 0, 20, 20);
        // scene->AddGUIToScene(canvas1);
        // canvas2 = new CanvasElement(0, 39, 20, 20);
        // scene->AddGUIToScene(canvas2);
        // canvas3 = new CanvasElement(0, 50, 20, 20);
        // scene->AddGUIToScene(canvas3);
        // canvas4 = new CanvasElement(0, 100, 20, 20);
        // scene->AddGUIToScene(canvas4);
        // canvas5 = new CanvasElement(10, 75, 20, 20);
        // scene->AddGUIToScene(canvas5);
        // canvas6 = new CanvasElement(30, 90, 20, 20);
        // scene->AddGUIToScene(canvas6);
        // canvas7 = new CanvasElement(50, 90, 20, 20);
        // scene->AddGUIToScene(canvas7);
        // canvas8 = new CanvasElement(400, 100, 20, 20);
        // scene->AddGUIToScene(canvas8);
        // canvas9 = new CanvasElement(450, 450, 20, 20);
        // scene->AddGUIToScene(canvas9);
        // canvas10 = new CanvasElement(450, 450, 20, 20);
        // scene->AddGUIToScene(canvas10);


        // canvasPrime = new CanvasElement(200, 150, 100, 100);
        // scene->AddGUIToScene(canvasPrime);
        

        // auto callback = [&](){
        //     canvas->ClearCanvas({255,255,255});
        // };

        // button = new ButtonElement(300, 450, 200, 100, callback);
        // scene->AddGUIToScene(button);

        TextElement * text = new TextElement("Hello\nWorld!");
        text->transform.position.x = GraphicConfig::windowWidth/2.0;
        text->transform.position.y = GraphicConfig::windowHeight/2.0f;
        scene->AddGUIToScene(text);


    }


};

int main(){

    srand(time(nullptr));

    GraphicConfig::vsync = false;
    GraphicConfig::zbuffer = true;
    GraphicConfig::windowHeight = 600;
    GraphicConfig::windowWidth = 800;
    GraphicConfig::windowTitle = "Application";

    // ApplicationConfig::internalGUIInteraction = false;

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
