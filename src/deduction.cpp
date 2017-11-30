#include <gtest/gtest.h>
#include <iostream>

//typeid 并不能返回 const refrence等信息，只能返回类型和指针类型
//只是在vs2017下测试，gcc尚未测试 fuck windows10 LSB 
class DA {};
TEST(deduction,typeid)
{
    int x = 1;
    const int cx = x;
    const int& rx = x;
    int* px = &x;
    DA da;
    const DA &da1 = da;
    std::cout << "type id:" << std::endl
        << typeid(x).name() << std::endl
        << typeid(cx).name() << std::endl
        << typeid(rx).name() << std::endl
        << typeid(px).name() << std::endl
        << typeid(da).name() << std::endl
        << typeid(da1).name() << std::endl;
}

/*
template <typename T>
void f(paramType param);
f(expr)
T的类型推导不仅仅取决于expr的类型，还取决于param的类型(paramType)
*/

template <typename T>
auto func1(T& n)
{
   std::cout << "type T: " << typeid(T).name() << " type param: " << typeid(n).name()<<std::endl;
   return std::make_pair(typeid(T).name(),typeid(T&).name());
}

TEST(deduction,param_type_ref_of_pointer1)
{
    int x = 27;
    const int cx = x;
    const  int& rx = x;
    func1(x);
    func1(cx);
    func1(rx);
    //EXPECT_EQ(std::make_pair("int","int"),func1(x));
    /*
    EXPECT_EQ("int",func1(cx));
    //这种情况下根据expr来推导T
    //cx的类型是一个引用，类型推倒时省略引用的部分，所以T的类型还是int 而不是 int&
    EXPECT_EQ("int",func1(rx));
    */
}


template <typename T>
auto func2(const T& n)
{
   std::cout << "type T: " << typeid(T).name() << " type param: " << typeid(n).name()<<std::endl;
   return std::make_pair(typeid(T).name(),typeid(T&).name());
}

TEST(deduction,param_type_ref_of_pointer)
{
    int x = 27;
    const int cx = x;
    const  int& rx = x;
    std::cout << "----type--- " << typeid(rx).name() << std::endl;
    func2(x);
    func2(cx);
    func2(rx);
    /*
    EXPECT_EQ("int",func2(x));
    EXPECT_EQ("int",func2(cx));
    EXPECT_EQ("int",func2(rx));
    */
}

