#include "gtest/gtest.h"
#include <cassert>
#include <functional>
#include <cstring>

#include "Color.h"
#include "Component.h"
#include "ComponentType.h"
#include "EventType.h"
#include "UIBuilder.h"
#include "IEventHandlingService.h"
#include "EventManager.h"
#include "Button.h"

int integerForTesting;
int integerForTesting2;
void onEventFunc(){
    integerForTesting++;
};
void event1(){
    integerForTesting2++;
};
void event2(){
    integerForTesting2 *= 2;
};
void event3(){
    integerForTesting2 = 0;
};

TEST(UIBuilderTests, AssignEventManagerTest1){
    //UIBuilder::AssignEventManager() test

    UIBuilder uIBuilder;
    EventManager* eventManager;

    eventManager = new EventManager();

    uIBuilder.AssignEventManager(eventManager);

    ASSERT_EQ(eventManager, uIBuilder.getEventManager());
}

TEST(UIBuilderTests, AssignEventTest1){
    //UIBuilder::AssignEvent() test

    UIBuilder uIBuilder;
    EventType eventType;
    int a,b;
    uIBuilder.AssignEvent(eventType, onEventFunc);

    integerForTesting = 1;
    onEventFunc();
    a = integerForTesting;

    integerForTesting = 1;
    uIBuilder.getEvents()[eventType]();
    b = integerForTesting;

    //if a == b its means that the same function was use during computing a and b.
    ASSERT_EQ( a, b);
}



TEST(UIBuilderTests, SetComponentTypeTest1){
    //UIBuilder::SetComponentType() test

    UIBuilder uIBuilder;
    ComponentType componentType = BUTTON;

    uIBuilder.SetComponentType(componentType);

    ASSERT_EQ(uIBuilder.getType(), componentType);
}

TEST(UIBuilderTests, SetDimensionsTest1){
    //UIBuilder::SetDimensions() test

    UIBuilder uIBuilder;
    float w,h;

    w=1;
    h=2;
    uIBuilder.SetDimensions(w, h);

    ASSERT_EQ(w, uIBuilder.getWidth());
    ASSERT_EQ(h, uIBuilder.getHeight());
}

TEST(UIBuilderTests, SetPositionTest1){
    //UIBuilder::SetPosition() test

    UIBuilder uIBuilder;
    float x,y;

    uIBuilder.SetPosition(x, y);
    ASSERT_EQ(x, uIBuilder.getX());
    ASSERT_EQ(y, uIBuilder.getY());
}

TEST(UIBuilderTests, SetColorTest1){
    //UIBuilder::SetColor() test

    UIBuilder uIBuilder;
    Color color;
    int r,g,b,a;
    color.R = 2;
    color.G = 1;
    color.B = 3;
    color.A = 7;

    uIBuilder.SetColor(color);

    r = uIBuilder.getColor().R;
    g = uIBuilder.getColor().G;
    b = uIBuilder.getColor().B;
    a = uIBuilder.getColor().A;
    ASSERT_EQ(r, color.R);
    ASSERT_EQ(g, color.G);
    ASSERT_EQ(b, color.B);
    ASSERT_EQ(a, color.A);
}

TEST(UIBuilderTests, SetTextTest1){
    //UIBuilder::SetText(() test

    UIBuilder uIBuilder;
    const char * str = "Bajtolina_ma_bitkota";
    int strLen = std::strlen(str);

    uIBuilder.SetText((char*)str , strLen);

    int isEqueal = std::strcmp(str, uIBuilder.getText());
    ASSERT_EQ(isEqueal, 0);
    ASSERT_EQ(strLen, uIBuilder.getTextLen());


}




TEST(UIBuilderTests, BuildTest1){
    //UIBuilder::Build(() test

    UIBuilder uIBuilder;
    Component* createdComponent;

    EventManager* iEventHandlingService;
    EventType newEventType1;
    EventType newEventType2;
    ComponentType componentType;
    float width, height, x, y;
    Color color;

    iEventHandlingService = new EventManager();
    componentType = BUTTON;
    width = 42;
    height = 44;
    x = 404;
    y = 4;

    color.R = 2;
    color.G = 3;
    color.B = 5;
    color.A = 7;

    newEventType1 = ONCLICK;
    newEventType2 = ONHOVER;

    uIBuilder.AssignEventManager(iEventHandlingService);
    uIBuilder.AssignEvent(newEventType1, event1);
    uIBuilder.AssignEvent(newEventType2, event2);
    uIBuilder.SetComponentType(componentType);
    uIBuilder.SetPosition(x, y);
    uIBuilder.SetColor(color);
    uIBuilder.SetDimensions(width, height);

    createdComponent = uIBuilder.Build();


    ASSERT_EQ(width, createdComponent->getWidth());
    ASSERT_EQ(height, createdComponent->getHeight() );
    ASSERT_EQ(x, createdComponent->getX());
    ASSERT_EQ(y, createdComponent->getY());
    ASSERT_EQ(color.R, createdComponent->getColor().R);
    ASSERT_EQ(color.G, createdComponent->getColor().G);
    ASSERT_EQ(color.B, createdComponent->getColor().B);
    ASSERT_EQ(color.A, createdComponent->getColor().A);
}


TEST(UIBuilderTests, BuildTest2){
    //UIBuilder::Build(() test

    UIBuilder uIBuilder;
    Component* createdComponent;

    EventManager* iEventHandlingService;
    EventType newEventType1;
    ComponentType componentType;
    float width, height, x, y;
    Color color;

    iEventHandlingService = new EventManager();
    componentType = CANVAS;
    width = 11;
    height = 13;
    x = 17;
    y = 19;

    color.R = 6;
    color.G = 6;
    color.B = 6;
    color.A = 6;

    newEventType1 = ONMOVE;

    uIBuilder.AssignEventManager(iEventHandlingService);
    uIBuilder.AssignEvent(newEventType1, event1);
    uIBuilder.SetComponentType(componentType);
    uIBuilder.SetPosition(x, y);
    uIBuilder.SetColor(color);
    uIBuilder.SetDimensions(width, height);

    createdComponent = uIBuilder.Build();


    ASSERT_EQ(width, createdComponent->getWidth());
    ASSERT_EQ(height, createdComponent->getHeight() );
    ASSERT_EQ(x, createdComponent->getX());
    ASSERT_EQ(y, createdComponent->getY());
    ASSERT_EQ(color.R, createdComponent->getColor().R);
    ASSERT_EQ(color.G, createdComponent->getColor().G);
    ASSERT_EQ(color.B, createdComponent->getColor().B);
    ASSERT_EQ(color.A, createdComponent->getColor().A);
}


TEST(UIBuilderTests, BuildTest3){
    //UIBuilder::Build(() test
    UIBuilder uIBuilder;
    Component* createdComponent;

    EventManager* iEventHandlingService;
    EventType newEventType1;
    EventType newEventType2;
    ComponentType componentType;
    float width, height, x, y;
    Color color;
    int textLen;

    const char * text = "Ala ma kota.";
    textLen = strlen(text);
    iEventHandlingService = new EventManager();
    componentType = TEXT;
    width = 10000010;
    height = 10001000;
    x = 4;
    y = 1000;

    color.R = 2;
    color.G = 3;
    color.B = 5;
    color.A = 7;

    newEventType1 = ONCLICK;
    newEventType2 = ONHOVER;

    uIBuilder.AssignEventManager(iEventHandlingService);
    uIBuilder.AssignEvent(newEventType1, event1);
    uIBuilder.AssignEvent(newEventType2, event2);
    uIBuilder.SetComponentType(componentType);
    uIBuilder.SetPosition(x, y);
    uIBuilder.SetColor(color);
    uIBuilder.SetDimensions(width, height);
    uIBuilder.SetText((char*)text, textLen);

    createdComponent = uIBuilder.Build();


    bool isCreatedComponentClear = (createdComponent == NULL);
    ASSERT_EQ(width, createdComponent->getWidth());
    ASSERT_EQ(height, createdComponent->getHeight() );
    ASSERT_EQ(x, createdComponent->getX());
    ASSERT_EQ(y, createdComponent->getY());
    ASSERT_EQ(color.R, createdComponent->getColor().R);
    ASSERT_EQ(color.G, createdComponent->getColor().G);
    ASSERT_EQ(color.B, createdComponent->getColor().B);
    ASSERT_EQ(color.A, createdComponent->getColor().A);

}
