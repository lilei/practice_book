#include <gtest/gtest.h>
#include <stdlib.h>

TEST(signed, unsighed_minus)
{
    unsigned int a = 1;
    unsigned int b = 2;
    std::cout << b - a << std::endl;

    //无符号数的减法是会溢出的,但是取绝对值却可以得到正确的结果
    std::cout << a - b << std::endl;


    std::cout << (double)a - (double)b << std::endl;

    std::cout << abs((double)a - (double)b) << std::endl;
    //std::cout << abs(a - b) << std::endl;
}

TEST(signed, mod)
{
    //负数是可以做模运算并得到正确结果的
    size_t a = 0;
    std::cout << (a - 1) % 256 << std::endl;
}