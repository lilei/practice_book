#include <gtest/gtest.h>
#include <new>


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
        static void* operator new(size_t size)
        {
            printf("operator new in class A\n");
            return ::operator new (size);
        }

        static void operator delete(void* ptr)
        {
            printf("operator delete in class A\n");
        }

        static void* operator new(size_t size,void* ptr)
        {
            printf("placement new in class A\n");
            return ptr;
        }

        static void operator delete(void* ptr,void*)
        {
            printf("placement delete in class A\n");
        }

    };

    char buf[4] = { 0 };
    auto a = new (&buf[0])A;
    delete a;

    auto b = new A;
    delete b;
}
