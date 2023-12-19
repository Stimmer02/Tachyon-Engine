#include "gtest/gtest.h"
#include <functional>
#include <cstring>

#include "Color.h"
#include "ComponentType.h"
#include "EventType.h"
#include "UIBuilder.h"
#include "IEventHandlingService.h"
#include "EventManager.h"
#include "Button.h"

TEST(UIBuilderTests, AssignEventManagerTest1){
    //UIBuilder::AssignEventManager() test

    UIBuilder uIBuilder;
    EventManager* eventManager;

    eventManager = new EventManager();

    uIBuilder.AssignEventManager(eventManager);

    ASSERT_EQ(eventManager, uIBuilder.getEventManager());
}



int integerForTesting;
void onEventFunc(){
    integerForTesting++;
};
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
    char* str = "Bajtolina_ma_bitkota";
    int strLen = std::strlen(str);

    uIBuilder.SetText(str , strLen);

    int isEqueal = std::strcmp(str, uIBuilder.getText());
    ASSERT_EQ(isEqueal, 0);
    ASSERT_EQ(strLen, uIBuilder.getTextLen());


}
