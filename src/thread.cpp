#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <iostream>

void foo()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"func foo"<<std::endl;
}

void bar()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<<"func bar"<<std::endl;
}

TEST(thread,thread)
{
    std::thread t1(foo);
    std::thread t2(bar);

    std::cout << "2 thread started"<<std::endl;
    t1.join();
    t2.join();
}

TEST(thread,lambda)
{
    auto f = []() 
    {
        std::cout<<"func f"<<std::endl;
    };

    auto b = []() 
    {
        std::cout<<"func b"<<std::endl;
    };

    std::thread t1(f);
    std::thread t2(b);
    t1.join();
    t2.join();
}
