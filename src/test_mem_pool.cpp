#include "mem_pool.h"
#include "smart_ptr.h"
#include <gtest/gtest.h>
#include <new>

class A
{
};
TEST(mem_pool,node_size)
{
    EXPECT_EQ(sizeof(void*),sizeof(mem_pool<A>));
}

TEST(mem_pool, default_size)
{
    mem_pool<A> pool;
    EXPECT_EQ(DEFALUT_SIZE,pool.free_size());
}

TEST(mem_pool, alloc)
{
    mem_pool<A> pool;
    for (int i = 0;i < DEFALUT_SIZE;i++)
    {
        void* ptr = pool.alloc();
        EXPECT_FALSE(NULL == ptr);
    }
    EXPECT_EQ(0,pool.free_size());
}

TEST(mem_pool, reserve)
{
    mem_pool<A> pool;
    for (int i = 0; i < DEFALUT_SIZE; i++)
    {
        void* ptr = pool.alloc();
    }
    EXPECT_EQ(0,pool.free_size());

    pool.alloc();
    EXPECT_EQ(DEFALUT_SIZE - 1,pool.free_size());
}

TEST(mem_pool, free)
{
    mem_pool<A> pool;
    EXPECT_EQ(DEFALUT_SIZE, pool.free_size());

    void* ptr1 = pool.alloc();
    EXPECT_EQ(DEFALUT_SIZE - 1, pool.free_size());


    pool.free(ptr1);
    EXPECT_EQ(DEFALUT_SIZE, pool.free_size());

    void* ptr2 = pool.alloc();
    EXPECT_EQ(DEFALUT_SIZE - 1, pool.free_size());

    EXPECT_EQ(ptr1,ptr2);
}

// coopration with smart_prt

class B :public shared_object
{
};

mem_pool<class B> pool;
void del_func(void* ptr)
{
    pool.free(ptr);
};
TEST(mem_pool,smartptr_alloc)
{
    void* ptr = pool.alloc();
    smart_ptr<B> b = smart_ptr<B>((new (ptr)B),del_func);
    EXPECT_EQ(b.get(),ptr);
}


TEST(mem_pool,smartptr_alloc1)
{
    void* ptr1 = pool.alloc();
    auto b = smart_ptr<B>((new (ptr1)B),del_func);
    EXPECT_EQ(b.get(),ptr1);

    b = NULL;

    void* ptr2 = pool.alloc();
    EXPECT_EQ(ptr1,ptr2);

    auto c = smart_ptr<B>((new (ptr2)B),del_func);
    EXPECT_EQ(c.get(),ptr2);
}
