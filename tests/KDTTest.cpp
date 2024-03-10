#include "gtest/gtest.h"
#include <cassert>
#include <functional>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include <chrono>

#include "Color.h"
#include "Component.h"
#include "ComponentType.h"
#include "EventType.h"
#include "UIBuilder.h"
#include "IEventHandlingService.h"
#include "EventManager.h"
#include "Button.h"
#include "KDTElement.h"
#include "KDT.h"

TEST(KDTreeTests, treeTest1){
    KDT* tree = new KDT();

    Component** componentsArray;
    std::vector<Component*> componentsVector;
    int compArrSize = 1000000;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;



    // auto time1 = std::chrono::high_resolution_clock::now();

    componentsArray = new Component*[compArrSize];

    for(int i = 0; i < compArrSize - 1; ++i){
        componentsArray[i] = new Button(rand() % XUpperBound + XLowerBoun, rand() % YUpperBound + YLowerBound, rand() % widthUpperBound + widthLowerBound, rand() % heightUpperBound + heightLowerBOund);
        componentsVector.push_back(componentsArray[i]);
    }
    componentsArray[compArrSize - 1] = new Button(50, 50, 50, 50);
    componentsVector.push_back(componentsArray[compArrSize - 1]);
    // auto time2 = std::chrono::high_resolution_clock::now();


    tree->buildTree(componentsVector);

    Component* cmp = tree->find(51, 51);
    //bool spr = *cmp == *componentsArray[compArrSize - 1];
    bool spr = 1;
    // auto time3 = std::chrono::high_resolution_clock::now();
    if(cmp == NULL){
        spr = false;
    }
    ASSERT_EQ(cmp->getX(), 50);
    ASSERT_TRUE(spr);
    delete tree;
}












