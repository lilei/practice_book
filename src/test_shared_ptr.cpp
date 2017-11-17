#include "smart_ptr.h"
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

TEST(smart_ptr,construct)
{
    smart_ptr<A> ptr;
}

TEST(smart_ptr,construct_by_raw_ptr)
{
    auto ptr = smart_ptr<A>(new A());
}

TEST(smart_ptr,copy_construct)
{
    smart_ptr<A> ptr1;
    auto ptr2 = ptr1;
}

TEST(smart_ptr,copy_assign)
{
    smart_ptr<A> ptr1 = smart_ptr<A>(new A());
    EXPECT_EQ(1,ptr1->ref_count());

    smart_ptr<A> ptr2;
    EXPECT_EQ(0,ptr2->ref_count());

    ptr2 = ptr1;
    EXPECT_EQ(2,ptr1->ref_count());
    EXPECT_EQ(2,ptr2->ref_count());
}

TEST(smart_ptr,copy_assign_with_null)
{
    smart_ptr<A> ptr = smart_ptr<A>(new A());
    EXPECT_EQ(1,ptr->ref_count());

    ptr = smart_ptr<A>(NULL);
    EXPECT_EQ(0,ptr->ref_count());
}

TEST(smart_ptr,copy_assign_with_raw_null)
{
    smart_ptr<A> ptr = smart_ptr<A>(new A());
    EXPECT_EQ(1,ptr->ref_count());

    ptr = NULL;
    EXPECT_EQ(0,ptr->ref_count());

    ptr = NULL;
    EXPECT_EQ(0,ptr->ref_count());
}

TEST(smart_ptr,assign_by_raw)
{
    smart_ptr<A> ptr;
    A* a = new A;
    ptr = a;
}


TEST(smart_ptr,bool)
{
    smart_ptr<A> ptr1;
    smart_ptr<A> ptr2 = smart_ptr<A>(new A());
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

TEST(smart_ptr,get)
{
    A* a = new A;
    smart_ptr<A> ptr(a);
    EXPECT_EQ(ptr.get(), a);
}

TEST(smart_ptr, dereference)
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

    smart_ptr<Foo> ptr = smart_ptr<Foo>(new Foo(10));
    EXPECT_EQ(10,ptr->print());
    EXPECT_EQ(10,(*ptr).print());
}

TEST(smart_ptr,compare_with_shared_ptr)
{
    A* a1 = new A;
    A* a2 = new A;
    smart_ptr<A> ptr1 = smart_ptr<A>(a1);
    smart_ptr<A> ptr2 = smart_ptr<A>(a1);
    smart_ptr<A> ptr3 = smart_ptr<A>(a2);
    EXPECT_TRUE(ptr1 == ptr2);
    EXPECT_FALSE(ptr1 == ptr3);
    EXPECT_TRUE(ptr1 != ptr3);


    EXPECT_TRUE(ptr2 == ptr1);
    EXPECT_FALSE(ptr3 == ptr1);
    EXPECT_TRUE(ptr3 != ptr1);
}

TEST(smart_ptr, compare_with_raw_ptr)
{
    A* a1 = new A;
    A* a2 = new A;
    smart_ptr<A> ptr = smart_ptr<A>(a1);
    EXPECT_TRUE(ptr == a1);
    EXPECT_FALSE(ptr == a2);
}

TEST(smart_ptr, compare_with_NULL)
{
    A* a = new A;
    smart_ptr<A> ptr(a);
    A*  null_ptr = NULL;
    EXPECT_TRUE(ptr == a);
    EXPECT_FALSE(ptr == null_ptr);
    EXPECT_FALSE(ptr == smart_ptr<A>(NULL));

    smart_ptr<A> ptr_null;
    EXPECT_TRUE(ptr_null == smart_ptr<A>(NULL));
}

TEST(smart_ptr, ref_count)
{
    A* a = new A;
    smart_ptr<A> ptr1 = smart_ptr<A>(a);
    EXPECT_EQ(1, ptr1->ref_count());
    smart_ptr<A> ptr2 = smart_ptr<A>(a);
    EXPECT_EQ(2, ptr1->ref_count());
    EXPECT_EQ(2, ptr2->ref_count());
}

TEST(smart_ptr, ref_count2)
{
    smart_ptr<A> ptr1 = smart_ptr<A>(new A);
    EXPECT_EQ(1, ptr1->ref_count());
    auto ptr2 = ptr1;
    EXPECT_EQ(2, ptr1->ref_count());
}
