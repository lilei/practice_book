#include "shared_ptr.h"
#include <gtest/gtest.h>

class A :public shared_object
{
public:
    A()
    {
    }
    ~A()
    {
    }
};

TEST(shared_ptr,construct)
{
    shared_ptr<A> ptr;
}

TEST(shared_ptr,construct_by_raw_ptr)
{
    auto ptr = shared_ptr<A>(new A());
}

TEST(shared_ptr,copy_construct)
{
    shared_ptr<A> ptr1;
    auto ptr2 = ptr1;
}

TEST(shared_ptr,copy_assign)
{
    shared_ptr<A> ptr1 = shared_ptr<A>(new A());
    shared_ptr<A> ptr2;
    ptr2 = ptr1;
}

TEST(shared_ptr,assign_by_raw)
{
    shared_ptr<A> ptr;
    A* a = new A;
    ptr = a;
}


TEST(shared_ptr,bool)
{
    shared_ptr<A> ptr1;
    shared_ptr<A> ptr2 = shared_ptr<A>(new A());
    if (ptr1)
    {
        EXPECT_TRUE(false);
    }
    else
    {
        EXPECT_TRUE(true);
    }
    if (ptr2)
    {
        EXPECT_TRUE(true);
    }
    else
    {
        EXPECT_TRUE(false);
    }
}

TEST(shared_ptr,get)
{
    A* a = new A;
    shared_ptr<A> ptr(a);
    EXPECT_EQ(ptr.get(), a);
}

TEST(shared_ptr, dereference)
{
    struct Foo: public shared_object
    {
        Foo(int in) : foo_(in) {}
        int print() const
        {
            return foo_;
        }

        int foo_;
    };

    shared_ptr<Foo> ptr = shared_ptr<Foo>(new Foo(10));
    EXPECT_EQ(10,ptr->print());
    EXPECT_EQ(10,(*ptr).print());
}

TEST(shared_ptr,compare_with_shared_ptr)
{
    A* a1 = new A;
    A* a2 = new A;
    shared_ptr<A> ptr1 = shared_ptr<A>(a1);
    shared_ptr<A> ptr2 = shared_ptr<A>(a1);
    shared_ptr<A> ptr3 = shared_ptr<A>(a2);
    EXPECT_TRUE(ptr1 == ptr2);
    EXPECT_FALSE(ptr1 == ptr3);
    EXPECT_TRUE(ptr1 != ptr3);


    EXPECT_TRUE(ptr2 == ptr1);
    EXPECT_FALSE(ptr3 == ptr1);
    EXPECT_TRUE(ptr3 != ptr1);
}

TEST(shared_ptr, compare_with_raw_ptr)
{
    A* a1 = new A;
    A* a2 = new A;
    shared_ptr<A> ptr = shared_ptr<A>(a1);
    EXPECT_TRUE(ptr == a1);
    EXPECT_FALSE(ptr == a2);
}

TEST(shared_ptr, compare_with_NULL)
{
    A* a = new A;
    shared_ptr<A> ptr(a);
    A*  null_ptr = NULL;
    EXPECT_TRUE(ptr == a);
    EXPECT_FALSE(ptr == null_ptr);
    EXPECT_FALSE(ptr == shared_ptr<A>(NULL));

    shared_ptr<A> ptr_null;
    EXPECT_TRUE(ptr_null == shared_ptr<A>(NULL));
}

TEST(shared_ptr, ref_count)
{
    A* a = new A;
    shared_ptr<A> ptr1 = shared_ptr<A>(a);
    EXPECT_EQ(1, ptr1->ref_count());
    shared_ptr<A> ptr2 = shared_ptr<A>(a);
    EXPECT_EQ(2, ptr1->ref_count());
    EXPECT_EQ(2, ptr2->ref_count());
}

TEST(shared_ptr, ref_count2)
{
    shared_ptr<A> ptr1 = shared_ptr<A>(new A);
    EXPECT_EQ(1, ptr1->ref_count());
    auto ptr2 = ptr1;
    EXPECT_EQ(2, ptr1->ref_count());
}
