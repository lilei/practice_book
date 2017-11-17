/*
If you need smartptr, but you do not have a c++11 complier
and hate importing the huge boost library,
you can take this tiny implement of shared_ptr.
Warning: it's non-thread-safe!(nor the std::shared_ptr)
*/

#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include <cstddef>

template <class Y>
class smart_ptr
{
public:
    smart_ptr() :ptr_(NULL) {}
    explicit smart_ptr(Y* ptr) 
    { 
        ptr_ = ptr;
        if (NULL != ptr)
        {
            ptr_->inc_ref();
        }
    }

    smart_ptr(const smart_ptr& right) 
    { 
        ptr_ = right.get();
        if (NULL != ptr_)
        {
            ptr_->inc_ref();
        }
    }

    smart_ptr& operator=(const smart_ptr& right)
    {
        ptr_->dec_ref();
        ptr_ = right.get();
        ptr_->inc_ref();
        return (*this);
    }

    smart_ptr& operator=(Y* ptr)
    {
        ptr_->dec_ref();
        ptr_ = ptr;
        ptr->inc_ref();
        return (*this);
    }

    virtual ~smart_ptr() 
    {
        if (NULL != ptr_)
        {
            ptr_->dec_ref();
        }
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
};

class shared_object
{
public:
    shared_object()
        :ref_count_(0) {}

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
        if (0 == ref_count_)
        {
            delete this;
        }
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

private:
    shared_object(const shared_object&);
    const shared_object& operator=(const shared_object&);
    size_t ref_count_;
};

#endif /* SHARED_PTR_H */
