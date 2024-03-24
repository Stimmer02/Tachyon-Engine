#include "gtest/gtest.h"

#include "AttributeContainer.h"

struct Position{
    float x;
    float y;
};

struct Transform{
    Position position;
    float rotation;
};

TEST(AttributeContainerTests, AddElementTest){

    AttributeContainer<Transform> transforms;

    Transform * transform0 = transforms.AddElement(0);

    ASSERT_NE(transform0, nullptr);

}

TEST(AttributeContainerTests, RemoveElementTest){

    AttributeContainer<Transform> transforms;

    Transform * transform0 = transforms.AddElement(0);

    ASSERT_NE(transform0, nullptr);

    transforms.RemoveElement(0);

    Transform * transform1 = transforms.GetElement(0);

    ASSERT_EQ(transform1, nullptr);

}

TEST(AttributeContainerTests, GetElementTest){

    AttributeContainer<Transform> transforms;

    Transform * transform0 = transforms.AddElement(0);

    ASSERT_NE(transform0, nullptr);

    Transform * transform1 = transforms.GetElement(0);

    ASSERT_EQ(transform0, transform1);

}

TEST(AttributeContainerTests, AddMultipleElementsTest){

    AttributeContainer<Transform> transforms;

    for(int i = 0 ; i < 1000; ++i)
        transforms.AddElement(i);

    for(int i = 0 ; i < 1000; ++i){
        Transform * transform = transforms.GetElement(i);
        ASSERT_NE(transform, nullptr);
    }

    for(int i = 0 ; i < 1000; ++i)
        transforms.RemoveElement(i);

    for(int i = 0 ; i < 1000; ++i){
        Transform * transform = transforms.GetElement(i);
        ASSERT_EQ(transform, nullptr);
    }

}

TEST(AttributeContainerTests, EditElementTest){

    AttributeContainer<Transform> transforms;

    Transform * transform0 = transforms.AddElement(0);

    transform0->rotation = 30.0f;

    Transform * transform1 = transforms.GetElement(0);

    ASSERT_EQ(transform0->rotation, transform1->rotation);

    transforms.RemoveElement(0);

}

