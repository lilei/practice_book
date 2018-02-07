#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    testing::GTEST_FLAG(filter) = "BitStream.ps";
    //testing::GTEST_FLAG(repeat) = 1000;
    testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

#if (defined _WIN32 || defined _WIN64)
    getchar();
#endif
}
