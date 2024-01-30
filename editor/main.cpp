#include "UIManager.h"
#include "MouseInputService.h"
#include "UIBuilder.h"
#include "EventManager.h"

int main(){

    int width = 800, height = 600;
    float aspectRatio = width/(float)height;
    const char * title = "Tachyon Engine";

    UIManager app(width, height, title, true);
    MouseInputService mouse;
    EventManager eventSystem;

    app.AssignInputHandlingService( (IInputHandler*)&mouse );
    app.AssignEventHandlingService( (IEventHandlingService*)&eventSystem );

    UIBuilder builder;
    builder.AssignEventManager( (IEventHandlingService*)&eventSystem );

    uint32_t counter = 0;

    auto Hello = [&counter](){
        fprintf(stdout, "Hello World : %d\n", counter);
        counter++;
    };

    Component * button = builder
                        .SetComponentType(BUTTON)
                        ->SetPosition(width/2.0f, height/2.0f)
                        ->SetDimensions(aspectRatio * 100.0f, aspectRatio * 50.0f)
                        ->SetTexture("resources/sprites/button.bmp")
                        ->AssignEvent(ONCLICK, Hello)
                        ->Build();

    app.AddComponentToScene(button);

    Component * canvas = builder
                        .SetComponentType(CANVAS)
                        ->SetPosition(width/2.0f, 3*height/4.0f)
                        ->SetDimensions(aspectRatio * 340.0f, aspectRatio * 80.0f)
                        ->SetTexture("resources/sprites/editor_logo.bmp")
                        ->Build();

    app.AddComponentToScene(canvas);

    while( !app.ShouldClose() ){

        app.Update();

    }

    return 0;
}
