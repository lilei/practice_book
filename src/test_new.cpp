#include <gtest/gtest.h>
#include <new>


//effective c++ 
//条款52：write placement delete if you write placement delete
TEST(new,override)
{
    class A 
    {
    public:
        A()
        {
        }
        ~A()
        {
        }
        static void* operator new(size_t size)
        {
            printf("operator new in class A\n");
            return ::operator new (size);
        }

        static void operator delete(void* ptr)
        {
            printf("operator delete in class A\n");
            ::operator delete (ptr);
        }
    };

    auto a = new A;
    delete a;
}

TEST(new,placement)
{
    class A 
    {
    public:
        A()
        {
        }
        ~A()
        {
        }

        //如果不写一个“正常的”new" 函数，则编译器就不认识 new A这样的语句了
        void* operator new(size_t size)
        {
            printf("operator new in class A\n");
            return ::operator new (size);
        }

        //即便调用的是placement　new,在调用delete的时候还是会调用它
        //placement delete只是在placment new成功，而构造函数抛出异常时，由C++ runtime调用
        //好像还没有办法由程序员调用placement new?
        void operator delete(void* ptr)
        {
            printf("operator delete in class A\n");
            ::operator delete (ptr);   
        }

        //placement new
        void* operator new(size_t size,void* ptr)
        {
            printf("placement new in class A\n");
            return ptr;
        }

        //placement delete
        void operator delete(void* ptr,void*)
        {
            printf("placement delete in class A\n");
        }
    }; 
    char buf[4] = { 0 };
    auto a = new (&buf[0])A;

    auto b = new A;
    delete b;
}
