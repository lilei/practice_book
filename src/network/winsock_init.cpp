/*
    如何优雅的写winsocket初始化
    写在main函数里？main函数根本没有socket操作，写起来不是很突兀？
    写在第一次使用网络的时候？代码规模越来越大鬼知道哪里是第一次

    下面是从webrtc代码抄来的一个小技巧:
    1. 利用static对象在main函数前自动构造的特性，构造一个静态对象，在对象的构造析构里搞事情
    2. 构造函数是private的，这样这个class就没有办法被使用啦
    3. 把单例声明在class内部，这样单例还是可以被构造的，因为这样构造函数对它就是accessble的
    4. 但是析构要写成public的，因为销毁它时需要析构的访问权(VS2017下写成private的也没有关系)
*/
#if defined(WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") 
class WinsockInit 
{
    static WinsockInit singleton;
    WinsockInit() 
    {
        WSADATA data;
        WSAStartup(MAKEWORD(1, 0), &data);
    }

public:
    ~WinsockInit() 
    {
        WSACleanup(); 
    }
};
WinsockInit WinsockInit::singleton;
#endif

#include <gtest/gtest.h>
TEST(winsock,construct)
{
    //WinsockInit init; // compile error
}
