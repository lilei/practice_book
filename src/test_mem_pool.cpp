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
mem_pool<class B> pool_b;
class B :public shared_object
{
    void destroy()
    {
        pool_b.free(this);
    }
};
TEST(mem_pool,smartptr_alloc)
{
    void* ptr = pool_b.alloc();
    smart_ptr<B> b = smart_ptr<B>(new (ptr)B);
    EXPECT_EQ(b.get(),ptr);
}


mem_pool<class C> pool_c;
class C :public shared_object
{
    void destroy()
    {
        pool_c.free(this);
    }
};
TEST(mem_pool,smartptr_free)
{
    EXPECT_EQ(DEFALUT_SIZE, pool_c.free_size());
    
    auto c1 = smart_ptr<C>(new (pool_c.alloc())C);
    EXPECT_EQ(DEFALUT_SIZE-1, pool_c.free_size());
    auto ptr1 = c1.get();
    
    c1 = NULL;
    EXPECT_EQ(DEFALUT_SIZE, pool_c.free_size());


    auto c2 = smart_ptr<C>(new (pool_c.alloc())C);
    EXPECT_EQ(DEFALUT_SIZE-1, pool_c.free_size());
    auto ptr2 = c2.get();

    EXPECT_EQ(ptr1,ptr2);
}
