#include "gtest/gtest.h"

#include "AttributeContainer.h"

TEST(AttributeContainerTests, AddElementTest){

    struct Position{
        float x;
        float y;
    };

    struct Transform{
        Position position;
        float rotation;
    };

    AttributeContainer<Transform> transforms;

    Transform * transform0 = transforms.AddElement(0);

    ASSERT_NE(transform0, nullptr);

}

TEST(AttributeContainerTests, RemoveElementTest){

    struct Position{
        float x;
        float y;
    };

    struct Transform{
        Position position;
        float rotation;
    };

    AttributeContainer<Transform> transforms;

    Transform * transform0 = transforms.AddElement(0);

    ASSERT_NE(transform0, nullptr);

    transforms.RemoveElement(0);

    Transform * transform1 = transforms.GetElement(0);

    ASSERT_EQ(transform1, nullptr);

}