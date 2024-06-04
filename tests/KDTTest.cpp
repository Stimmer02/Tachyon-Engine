#include "gtest/gtest.h"
#include <cassert>
#include <functional>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>
#include <chrono>

#include "EventType.h"
#include "UIBuilder.h"
#include "IEventHandlingService.h"
#include "EventManager.h"
#include "ButtonElement.h"
#include "WindowContext.h"
#include "KDTElement.h"
#include "KDT.h"

TEST(KDTreeTests, treeTest1){

    // It needs to be somewhere in this code to allow OpenGL operations in tests
    WindowContext ctx(true);
    ctx.Open();

    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1000;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;



    // auto time1 = std::chrono::high_resolution_clock::now();

    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){
        printf("OK");
    };

    for(int i = 0; i < compArrSize - 1; ++i){
        componentsArray[i] = new ButtonElement(rand() % widthUpperBound + widthLowerBound, rand() % heightUpperBound + heightLowerBOund, callback);
        componentsArray[i]->transform.position = Vector3(rand() % XUpperBound + XLowerBoun, rand() % YUpperBound + YLowerBound, 0);
        componentsVector.push_back(componentsArray[i]);
    }
    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);
    // auto time2 = std::chrono::high_resolution_clock::now();


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(51, 51);
    //bool spr = *cmp == *componentsArray[compArrSize - 1];
    bool spr = 1;
    // auto time3 = std::chrono::high_resolution_clock::now();
    if(cmp == NULL){
        spr = false;
    }
    ASSERT_EQ(cmp->transform.position.x, 50);
    ASSERT_TRUE(spr);
    delete tree;

    for(int i = 0; i < compArrSize - 1; ++i){
        delete componentsArray[i];
    }
    delete[] componentsArray;

    ctx.Close();
}



TEST(KDTreeTests, treeTest2){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;


    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};

    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(51, 51);

    bool spr = 1;

    if(cmp == NULL){
        spr = false;
    }
    ASSERT_EQ(cmp->transform.position.x, 50);
    ASSERT_TRUE(spr);
    delete tree;
}


TEST(KDTreeTests, treeTest3){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;


    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};

    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(50, 50);

    bool spr = 1;

    if(cmp == NULL){
        spr = false;
    }
    ASSERT_EQ(cmp->transform.position.x, 50);
    ASSERT_TRUE(spr);
    delete tree;
}


TEST(KDTreeTests, treeTest4){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;


    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};

    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(100, 100);

    bool spr = 1;

    if(cmp == NULL){
        spr = false;
    }
    ASSERT_EQ(cmp->transform.position.x, 50);
    ASSERT_TRUE(spr);
    delete tree;
}

TEST(KDTreeTests, treeTest5){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;

    auto callback = [](){};

    componentsArray = new InteractiveElement*[compArrSize];


    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(101, 100);

    bool spr = 1;

    if(cmp != NULL){
        spr = false;
    }

    ASSERT_TRUE(spr);
    delete tree;
}


TEST(KDTreeTests, treeTest6){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;


    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};

    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(100, 101);

    bool spr = 1;

    if(cmp != NULL){
        spr = false;
    }
    ASSERT_TRUE(spr);
    delete tree;
}


TEST(KDTreeTests, treeTest7){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;


    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};


    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(49, 100);

    bool spr = 1;

    if(cmp != NULL){
        spr = false;
    }
    ASSERT_TRUE(spr);
    delete tree;
}


TEST(KDTreeTests, treeTest8){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;

    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};

    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(100, 49);

    bool spr = 1;

    if(cmp != NULL){
        spr = false;
    }
    ASSERT_TRUE(spr);
    delete tree;
}


TEST(KDTreeTests, treeTest9){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1000;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;



    // auto time1 = std::chrono::high_resolution_clock::now();

    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};

    for(int i = 0; i < compArrSize/2; ++i){
        componentsArray[i] = new ButtonElement(rand() % widthUpperBound + widthLowerBound, rand() % heightUpperBound + heightLowerBOund, callback);
        componentsArray[i]->transform.position = Vector3(rand() % XUpperBound + XLowerBoun, rand() % YUpperBound + YLowerBound, 0);
        componentsVector.push_back(componentsArray[i]);
    }
    for(int i = compArrSize/2; i < compArrSize - 1; ++i){
        componentsArray[i] = new ButtonElement(rand() % widthUpperBound + widthLowerBound, rand() % heightUpperBound + heightLowerBOund, callback);
        componentsArray[i]->transform.position = Vector3(rand() % XUpperBound + XLowerBoun, rand() % YUpperBound + YLowerBound, 0);
        componentsVector.push_back(componentsArray[i]);
    }
    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);
    // auto time2 = std::chrono::high_resolution_clock::now();


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(51, 51);
    //bool spr = *cmp == *componentsArray[compArrSize - 1];
    bool spr = 1;
    // auto time3 = std::chrono::high_resolution_clock::now();
    if(cmp == NULL){
        spr = false;
    }
    ASSERT_EQ(cmp->transform.position.x, 50);
    ASSERT_TRUE(spr);
    delete tree;
}

TEST(KDTreeTests, treeTest10){
    KDT* tree = new KDT();

    InteractiveElement** componentsArray;
    std::vector<InteractiveElement*> componentsVector;
    int compArrSize = 1000;

    const int XLowerBoun = 10, XUpperBound = 10, YLowerBound = 10, YUpperBound = 10, widthLowerBound = 10, widthUpperBound = 10, heightLowerBOund = 10, heightUpperBound = 10;



    // auto time1 = std::chrono::high_resolution_clock::now();

    componentsArray = new InteractiveElement*[compArrSize];

    auto callback = [](){};

    for(int i = 0; i < compArrSize/2; ++i){
        componentsArray[i] = new ButtonElement(rand() % widthUpperBound + widthLowerBound, rand() % heightUpperBound + heightLowerBOund, callback);
        componentsArray[i]->transform.position = Vector3(rand() % XUpperBound + XLowerBoun, rand() % YUpperBound + YLowerBound, 0);
        componentsVector.push_back(componentsArray[i]);
    }
    for(int i = compArrSize/2; i < compArrSize - 1; ++i){
        componentsArray[i] = new ButtonElement(rand() % widthUpperBound + widthLowerBound, rand() % heightUpperBound + heightLowerBOund, callback);
        componentsArray[i]->transform.position = Vector3(rand() % XUpperBound + XLowerBoun, rand() % YUpperBound + YLowerBound, 0);
        componentsVector.push_back(componentsArray[i]);
    }
    componentsArray[compArrSize - 1] = new ButtonElement(50, 50, callback);
    componentsArray[compArrSize - 1]->transform.position = Vector3(50, 50, 0);
    componentsVector.push_back(componentsArray[compArrSize - 1]);
    // auto time2 = std::chrono::high_resolution_clock::now();


    tree->buildTree(componentsVector);

    InteractiveElement* cmp = tree->find(49, 49);
    //bool spr = *cmp == *componentsArray[compArrSize - 1];
    bool spr = 1;
    // auto time3 = std::chrono::high_resolution_clock::now();
    if(cmp != NULL){
        spr = false;
    }
    ASSERT_TRUE(spr);
    delete tree;
}

