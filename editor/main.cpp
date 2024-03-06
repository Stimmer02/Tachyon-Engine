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
    // MouseInputService mouse;
    // EventManager eventSystem;

    // app.AssignInputHandlingService( (IInputHandler*)&mouse );
    // app.AssignEventHandlingService( (IEventHandlingService*)&eventSystem );


    // UIBuilder builder;
    // builder.AssignEventManager( (IEventHandlingService*)&eventSystem );

    // uint32_t counter = 0;

    // auto Hello = [&counter](){
    //     fprintf(stdout, "Hello World : %d\n", counter);
    //     counter++;
    // };

    //Image temp = BitmapReader::ReadFile("resources/sprites/charmap-oldschool_white.bmp");

    // ImageEditor ie;

    // uint32_t helperSize;
    // Image* helper = ie.Split(temp, 7, 9, helperSize);

    // Sprite ** buttonImg = new Sprite*[helperSize];


    // for(int i = 0; i < helperSize; ++i)
    //     buttonImg[i] = Sprite::Create(helper + i);
    


    // for(int i = 0; i < helperSize; ++i){

    //         Component * button = builder
    //                     .SetComponentType(BUTTON)
    //                     ->SetPosition((i%18) * 30.0f + 100, -(i/18) * 40.0f  + 400)
    //                     ->SetDimensions(28.0f, 36.0f)
    //                     ->SetTexture(buttonImg[i])
    //                     ->AssignEvent(ONCLICK, Hello)
    //                     ->SetColor((Color){27, 54, 56})
    //                     ->Build();

    //     app.AddComponentToScene(button);
    // }
    // Sprite* buttonImg2 = Sprite::Create(&temp);
    //         Component * button = builder
    //                     .SetComponentType(BUTTON)
    //                     ->SetPosition((width)/2.0f, (height)/2.0f)
    //                     ->SetDimensions(100.0f, 50.0f)
    //                     ->SetTexture(buttonImg2)
    //                     ->AssignEvent(ONCLICK, Hello)
    //                     ->SetColor((Color){27, 54, 56, 255})
    //                     ->Build();
    //
    //     app.AddComponentToScene(button);



    // Sprite * buttonImg = Sprite::Create(&temp);
    //
    // Component * button = builder
    //                     .SetComponentType(BUTTON)
    //                     ->SetPosition(width/2.0f, height/2.0f)
    //                     ->SetDimensions(100.0f, 50.0f)
    //                     ->SetTexture(buttonImg)
    //                     ->AssignEvent(ONCLICK, Hello)
    //                     ->SetColor((Color){27, 54, 56, 255})
    //                     ->Build();


    while( !app.ShouldClose() ){

        app.Update();

    }

    // for(int i = 0; i < helperSize; ++i){
    //     delete buttonImg[i];
    // }
    // delete [] buttonImg;



    return 0;
}
