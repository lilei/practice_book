#include <gtest/gtest.h>
#include <iostream>

//typeid �����ܷ��� const refrence����Ϣ��ֻ�ܷ������ͺ�ָ������
//ֻ����vs2017�²��ԣ�gcc��δ���� fuck windows10 LSB 
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
T�������Ƶ�������ȡ����expr�����ͣ���ȡ����param������(paramType)
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
    //��������¸���expr���Ƶ�T
    //cx��������һ�����ã������Ƶ�ʱʡ�����õĲ��֣�����T�����ͻ���int ������ int&
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

