#include <gtest/gtest.h>

TEST(lambda,capture_ref)
{
    int a = 1;
    int b = 2;
    int c = 3;
    [&]() 
    {
        EXPECT_EQ(1,a);
        EXPECT_EQ(2,b);
        EXPECT_EQ(3,c);

        ++a;
        ++b;
        ++c;
    }();

    EXPECT_EQ(2,a);
    EXPECT_EQ(3,b);
    EXPECT_EQ(4,c);
}

TEST(lambda,capture_value)
{
    int a = 1;
    int b = 2;
    int c = 3;
    [](int a,int b,int c) 
    {
        ++a;
        EXPECT_EQ(2,a);
        EXPECT_EQ(2,b);
        EXPECT_EQ(3,c);
    }(a,b,c);

    EXPECT_EQ(1,a);
    EXPECT_EQ(2,b);
    EXPECT_EQ(3,c);
}
