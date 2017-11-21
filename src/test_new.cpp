#include <gtest/gtest.h>
#include <new>


//effective c++ 
//����52��write placement delete if you write placement delete
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

        //�����дһ���������ġ�new" ��������������Ͳ���ʶ new A�����������
        void* operator new(size_t size)
        {
            printf("operator new in class A\n");
            return ::operator new (size);
        }

        //������õ���placement��new,�ڵ���delete��ʱ���ǻ������
        //placement deleteֻ����placment new�ɹ��������캯���׳��쳣ʱ����C++ runtime����
        //����û�а취�ɳ���Ա����placement new?
        void operator delete(void* ptr)
        {
            printf("operator delete in class A\n");
            ::operator delete (ptr);   
        }

        //placement new
        void* operator new(size_t size,void* ptr)
        {
            printf("placement new in class A\n");
            return ptr;
        }

        //placement delete
        void operator delete(void* ptr,void*)
        {
            printf("placement delete in class A\n");
        }
    }; 
    char buf[4] = { 0 };
    auto a = new (&buf[0])A;

    auto b = new A;
    delete b;
}
