#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    testing::GTEST_FLAG(filter) = "search.*";
    //testing::GTEST_FLAG(repeat) = 1000;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
