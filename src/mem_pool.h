#ifndef MEM_POOL_H
#define MEM_POOL_H

/*
An object memory pool.
Every node of the pool contains either the payload or the pointer of the next node.
When every node's lengh is less than a pointer's,It's cost nothing extra but the memory itself.
*/

const size_t DEFALUT_SIZE = 2048;

template <class T>
class mem_pool
{
public:
    mem_pool();
    ~mem_pool();

    void* alloc();

    void free(void* recycle);

private:
    void reserve(size_t size = DEFALUT_SIZE);

    mem_pool<T>* next_; /*be sure of this class has this only member*/
};

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
        next_ = next_->next_;
        delete[] ptr;
    }
}
template <class T>
void* mem_pool<T>::alloc()
{
    if (NULL == next)
    {
        reserve();
    }
    mem_pool<T>* got = next_;
    next_ = got->next_;
    return got;
}

template <class T>
void mem_pool<T>::free(void* recycle)
{
    mem_pool<T>* new_head = static_cast<mem_pool<T>*>(recycle);

    new_head->next = next_;
    next_ = new_head;
}

template <class T>
void mem_pool<T>::reserve(size_t size /*= DEFALUT_SIZE*/)
{
    size_t len = (sizeof(T) > sizeof(void*)) ? sizeof(T) : sizeof(void*);
    mem_pool<T>* ptr = static_cast<mem_pool<T>*>(new char[len]);
    next_ = ptr;

    for (size_t i = 0;i < size;i++)
    {
        ptr->next_ = static_cast<mem_pool<T>*>(new char[len]);
        ptr = ptr->next_;
    }
    ptr->next_ = NULL;
}

#endif /* MEM_POOL_H */
