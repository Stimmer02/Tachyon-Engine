#include "gtest/gtest.h"

#include "InteractiveComponent.h"

TEST(MyTestSuite, MyTest) {

    ASSERT_TRUE(true);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
