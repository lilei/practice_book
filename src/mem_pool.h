#ifndef MEM_POOL_H
#define MEM_POOL_H

/*
An object memory pool.
When a node of the pool is unused,we reuse it to store the next node's point.
The extra memory cost is only sizeof(void*).
*/


#include <cstddef>

const size_t DEFALUT_SIZE = 1;

template <class T>
class mem_pool
{
public:
    mem_pool();
    ~mem_pool();

    void* alloc();

    void free(void* recycle);

    //only for debug
    size_t free_size();
private:
    void reserve(size_t size = DEFALUT_SIZE);

    mem_pool<T>* next_; /*be sure of this class has this only member*/
};

template <class T>
size_t mem_pool<T>::free_size()
{
    size_t i = 0;
    mem_pool<T>* ptr = next_;
    while (NULL != ptr)
    {
        ++i;
        ptr = ptr->next_;
    }
    return i;
}

template <class T>
mem_pool<T>::mem_pool()
    :next_(NULL)
{
    reserve();
}

template <class T>
mem_pool<T>::~mem_pool()
{
    while (NULL != next_)
    {
        mem_pool<T>* ptr = next_;
        next_ = ptr->next_;
        delete[] reinterpret_cast<char*>(ptr);
    }
}

template <class T>
void* mem_pool<T>::alloc()
{
    if (NULL == next_)
    {
        reserve();
    }
    mem_pool<T>* got = next_;
    next_ = got->next_;
    return static_cast<void*>(got);
}

template <class T>
void mem_pool<T>::free(void* recycle)
{
    mem_pool<T>* new_head = reinterpret_cast<mem_pool<T>*>(recycle);

    new_head->next_ = next_;
    next_ = new_head;
}

template <class T>
void mem_pool<T>::reserve(size_t size)
{
    size_t len = (sizeof(T) > sizeof(void*)) ? sizeof(T) : sizeof(void*);
    mem_pool<T>* ptr = this;
    for (size_t i = 0;i < size;i++)
    {
        ptr->next_ = reinterpret_cast<mem_pool<T>*>(new char[len]);
        ptr = ptr->next_;
    }
    ptr->next_ = NULL;
}

#endif /* MEM_POOL_H */
