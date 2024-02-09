#include "Sprite.h"
#include "UIManager.h"
#include "MouseInputService.h"
#include "UIBuilder.h"
#include "EventManager.h"
#include "ImageEditor.h"
#include <unistd.h>
#include <cmath>

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

    TextAssembler assembler;

    Image image = BitmapReader::ReadFile("resources/sprites/charset.bmp");
    std::vector<Image> letters = ImageEditor::Split(image, 7, 9);
    std::vector<Component *> handle;
    delete[] image.pixels;
    assembler.CreateCharset(letters.data(), letters.size());

    bool done = false;

    auto counterFunc = [&app, &assembler, &done, &builder, &handle, aspectRatio, height](){
        if( done == true )
            return;

        const char * text = "> What should i do ?";

        std::vector<Sprite *> textSprites = assembler.BuildText(text);

        for(uint32_t i = 0; i< textSprites.size(); ++i){

            Component * letter = builder
                                .SetComponentType(CANVAS)
                                ->SetPosition(20.0f + i * 15, height-20.0f)
                                ->SetDimensions(aspectRatio * 10.0f, aspectRatio * 20.0f)
                                ->SetTexture(textSprites[i])
                                ->Build();

            app.AddComponentToScene(letter);
            handle.emplace_back(letter);
        }

        done = true;
    };

    auto exitFunc = [&app](){
        app.Close();
    };

    Component * load = builder
                        .SetComponentType(BUTTON)
                        ->SetPosition(width/2.0f, height/2.0f)
                        ->SetDimensions(aspectRatio * 100.0f, aspectRatio * 50.0f)
                        ->SetTexture("resources/sprites/button_load.bmp")
                        ->AssignEvent(ONCLICK, counterFunc)
                        ->Build();

     app.AddComponentToScene(load);

     Component * exit = builder
                        .SetComponentType(BUTTON)
                        ->SetPosition(width/2.0f, height/2.0f - aspectRatio * 50.0f - 30.0f)
                        ->SetDimensions(aspectRatio * 100.0f, aspectRatio * 50.0f)
                        ->SetTexture("resources/sprites/button_exit.bmp")
                        ->AssignEvent(ONCLICK, exitFunc)
                        ->Build();

     app.AddComponentToScene(exit);

    Component * canvas = builder
                        .SetComponentType(CANVAS)
                        ->SetPosition(width/2.0f, 3*height/4.0f)
                        ->SetDimensions(aspectRatio * 340.0f, aspectRatio * 80.0f)
                        ->SetTexture("resources/sprites/editor_logo.bmp")
                        ->Build();

    app.AddComponentToScene(canvas);

    float angle = 0.0f;

    while( !app.ShouldClose() ){

        app.Update();

        float offset = 0.0f;

        for( auto comp : handle){
            float x = comp->getX();

            float y = sin(angle * 3.1415926535f/180.0f + offset)*5 + height-20.0f;

            comp->SetPosition(x, y);

            offset+=45.0f;
        }

        angle = (angle<360.0f)*(angle+5.0f);
    }

    return 0;
}
