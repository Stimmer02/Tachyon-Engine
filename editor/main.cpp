#include "Sprite.h"
#include "UIManager.h"
#include "MouseInputService.h"
#include "UIBuilder.h"
#include "BitmapReader.h"
#include "EventManager.h"
#include "ImageEditor.h"
#include <algorithm>

int main(){

    int width = 1280, height = 960;
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

    Image temp = BitmapReader::ReadFile("resources/sprites/button.bmp");

    Sprite * buttonImg = Sprite::Create(temp.pixels, temp.width, temp.height);

    if( buttonImg != nullptr){

        Component * button = builder
                        .SetComponentType(BUTTON)
                        ->SetPosition(width/2.0f, height/2.0f)
                        ->SetDimensions(100.0f, 50.0f)
                        ->SetTexture(buttonImg)
                        ->AssignEvent(ONCLICK, Hello)
                        ->SetColor((Color){27, 54, 56})
                        ->Build();

        app.AddComponentToScene(button);
    }

    while( !app.ShouldClose() ){

        app.Update();

    }



    return 0;
}
