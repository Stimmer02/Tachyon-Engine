#include "UIManager.h"
#include "MouseInputService.h"
#include "UIBuilder.h"

int main(){

    int width = 640, height = 480;
    const char * title = "Tachyon Engine";

    UIManager manager(width, height, title, true);
    MouseInputService mouse;
    UIBuilder builder;

    Component * button = builder
                        .SetComponentType(BUTTON)
                        ->SetPosition(width/2.0f, height/2.0f)
                        ->SetDimensions(50.0f, 25.0f)
                        ->Build();

    manager.AssignInputHandlingService( (IInputHandler*)&mouse );
    manager.AddComponentToScene(button);

    while( !manager.ShouldClose() ){

        manager.Update();

    }

    return 0;
}
