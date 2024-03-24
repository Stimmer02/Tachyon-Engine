#include "gtest/gtest.h"

#include "AttributeContainer.h"
#include "AttributeContainer.cpp"

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