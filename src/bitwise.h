#ifndef BITWISE_H
#define BITWISE_H
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <iostream>

class bitwise
{
public:
    bitwise(char* data,int len)
        :data_(data),len_(len)
    {}

    ~bitwise()
    {}

    void set(int offset)
    {
        int pos = 0;
        char* start = goto_pos(offset,pos);
        *start |= 1u << (7 - pos);
    }

    void clear(int offset)
    {
        int pos = 0;
        char* start = goto_pos(offset,pos);
        *start &= ~(1u << (7 - pos));
    }

    void toggle(int offset) 
    {
        int pos = 0;
        char* start = goto_pos(offset,pos);
        *start ^= 1u << (7 - pos);
    }

    bool check(int offset) 
    {
        int pos = 0;
        char* start = goto_pos(offset,pos);
        return (*start >> (7 - pos)) & 1u;
    }

    int size()
    {
        return len_;
    }

    template<typename T>
    T to_number(int offset,int len)
    {
        assert(len <= sizeof(T) * 8);
        T number = 0;
        bitwise n((char*)&number,sizeof(T));
        for (int i = 0; i < len; i++)
        {
            if (check(i + offset))
            {
                n.set(i + sizeof(T) * 8 - len);
            }
        }
        reverse_byte((char*)&number, sizeof(number));
        return number;
    }


    static void swap(char &left, char &right)
    {
        char temp = left;
        left = right;
        right = temp;
    }
    static void reverse_byte(char* data, int len)
    {
       for (int i = 0;i < len /2;i++)
       {
           swap(*(data + i), *(data + len - 1 - i));
       }
    }

private:
    char* goto_pos(int offset,int &pos)
    {
        char* start = data_ + offset / 8;
        pos = offset % 8;
        return start;
    }

    char* data_;
    int len_;
};


#endif /* BITWISE_H */
