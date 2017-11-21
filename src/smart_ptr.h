/*
If you need smartptr, but you do not have a c++11 complier
and hate importing the huge boost library,
you can take this tiny implement of shared_ptr.
Warning: it's non-thread-safe!(nor the std::shared_ptr)
*/

#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include <cstddef>

typedef void(*deleter)(void* ptr);

template <class Y>
class smart_ptr
{
public:
    smart_ptr()
        :ptr_(NULL),
        deleter_(NULL)
    {
    }

    explicit smart_ptr(Y* ptr) 
        :ptr_(ptr),
        deleter_(NULL)
    { 
        inc_ref();
    }

    smart_ptr(Y* ptr,deleter del_func) 
        :ptr_(ptr),
        deleter_(del_func)
    { 
        inc_ref();
    }

    smart_ptr(const smart_ptr& right) 
    { 
        ptr_ = right.get();
        inc_ref();
    }

    smart_ptr& operator=(const smart_ptr& right)
    {
        dec_ref();
        ptr_ = right.get();
        inc_ref();
        return (*this);
    }

    smart_ptr& operator=(Y* ptr)
    {
        dec_ref();
        ptr_ = ptr;
        inc_ref();
        return (*this);
    }

    virtual ~smart_ptr() 
    {
        dec_ref();
    }

    Y* get() const
    {
        return ptr_;
    }

    operator bool()
    {
        return NULL != ptr_;
    }

    Y& operator*() const
    {
        return *ptr_;
    }

    Y* operator->() const
    {
        return ptr_;
    }

    void inc_ref()
    {
        if (NULL != ptr_)
        {
            ptr_->inc_ref();
        }
    }

    void dec_ref()
    {
        if (NULL != ptr_)
        {
            ptr_->dec_ref();
            if (0 == ptr_->ref_count() )
            {
                if (NULL != deleter_)
                {
                    deleter_(ptr_);
                }
                else
                {
                    delete ptr_;
                }
                ptr_ = NULL;
            }
        }
    }

    bool operator==(const smart_ptr& right)const
    {
        return (this->ptr_ == right.get());
    }

    bool operator==(Y* right)const
    {
        return (this->ptr_ == right);
    }

    bool operator!=(const smart_ptr& right)const
    {
        return (this->ptr_ != right.get());
    }

private:
    Y* ptr_;
    deleter deleter_;
};

typedef void (*deleter)(void* ptr);
class shared_object
{
public:
    shared_object()
        :ref_count_(0)
    {
    }


    virtual ~shared_object() {}
    
    void inc_ref()
    {
        if (NULL != this)
        {
            ++ref_count_;
        }
    }

    void dec_ref()
    {
        if (NULL == this || 0 == ref_count_)
        {
            return;
        }
        --ref_count_;
    }

    size_t ref_count() const
    {
        if (NULL == this)
        {
            return 0;
        }
        else
        {
            return ref_count_;
        }
    }

    static void* operator new(size_t size)
    {
        return ::operator new (size);
    }

    static void operator delete(void* ptr)
    {
        ::operator delete(ptr);
    }

    static void* operator new(size_t size,void* ptr)
    {
        return ptr;
    }

    static void operator delete(void* ptr, void*)
    {
        return;
    }

private:
    shared_object(const shared_object&);
    const shared_object& operator=(const shared_object&);
    size_t ref_count_;
};

#endif /* SHARED_PTR_H */
