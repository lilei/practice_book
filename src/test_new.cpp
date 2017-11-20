#include <gtest/gtest.h>


TEST(new,override)
{
    class A 
    {
    public:
        A()
        {
            b_ = false;
        }
        ~A()
        {
        }
        void* operator new(size_t size)
        {
            return ::operator new (size);
        }

        bool b_;
    };

    auto a = new A;
}