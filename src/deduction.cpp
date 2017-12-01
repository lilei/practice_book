#include <gtest/gtest.h>
#include <iostream>

TEST(deduction,IDE_cursor_over)
{
    const int a = 42;
    auto x = a;
    auto y = &a;
}

//typeid用于在运行期获取类型信息
//但typeid 并不能返回 const refrence等信息，只能返回类型和指针类型
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

//所以我们用下面这种方式来在编译器
//声明一个模板类TD，但不定义
//编译器会在编译器错误里给出x,y的准确类型
template<typename T>
class TD;
TEST(deduction,complier_diag)
{
    const int a = 42;
    auto x = a;
    auto y = &a;

    /*
    TD<decltype(x)> xType;
    TD<decltype(y)> xType;
    */
}


/*
template <typename T>
void f(paramType param);
f(expr)
T的类型推导不仅仅取决于expr的类型，还取决于param的类型(paramType)
类型推导有如下三种情况:
case 1.paramType是引用或指针,但不是Universal引用
case 2.paramType是Universal 引用
case 3.ParamType不是指针也不是引用
*/

template <typename T>
void func1(T& n)
{
   //TD<decltype(T)> xType;
   TD<decltype(n)> yType;
}
TEST(deduction,paramtype_case1)
{
    int x = 27;
    const int cx = x;
    const  int& rx = x;
    //func1(x);      //T: int parameType: int
    //func1(cx);     //T: int parameType: const int
    func1(rx);     //T: int parameType: const int

    //这种情况下根据expr来推导T
    //cx的类型是一个引用，类型推倒时省略引用的部分，所以T的类型还是int 而不是 int&
}


template <typename T>
auto func2(const T& n)
{
   TD<decltype(T)> xType;
   TD<decltype(n)> yType;
}

TEST(deduction,paramtype_case2)
{
    int x = 27;
    const int cx = x;
    const  int& rx = x;
    std::cout << "----type--- " << typeid(rx).name() << std::endl;
    //func2(x);      //T: int parameType: int
    //func2(cx);     //T: int parameType: int
    //func2(rx);     //T: int parameType: int
}

