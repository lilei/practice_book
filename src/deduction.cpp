#include <gtest/gtest.h>
#include <iostream>

TEST(deduction,IDE_cursor_over)
{
    const int a = 42;
    auto x = a;
    auto y = &a;
}

//typeid�����������ڻ�ȡ������Ϣ
//��typeid �����ܷ��� const refrence����Ϣ��ֻ�ܷ������ͺ�ָ������
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

//�����������������ַ�ʽ���ڱ�����
//����һ��ģ����TD����������
//���������ڱ��������������x,y��׼ȷ����
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
T�������Ƶ�������ȡ����expr�����ͣ���ȡ����param������(paramType)
�����Ƶ��������������:
case 1.paramType�����û�ָ��,������Universal����
case 2.paramType��Universal ����
case 3.ParamType����ָ��Ҳ��������
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

    //��������¸���expr���Ƶ�T
    //cx��������һ�����ã������Ƶ�ʱʡ�����õĲ��֣�����T�����ͻ���int ������ int&
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

