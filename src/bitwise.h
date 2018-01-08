#ifndef BITWISE_H
#define BITWISE_H
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <iostream>

class bitwise
{
public:
    bitwise(unsigned char* data,int len)
        :data_(data),len_(len)
    {}

    ~bitwise()
    {}

    void set(int offset)
    {
        int pos = 0;
        unsigned char* start = goto_pos(offset,pos);
        *start |= 1u << (7 - pos);
    }

    void clear(int offset)
    {
        int pos = 0;
        unsigned char* start = goto_pos(offset,pos);
        *start &= ~(1u << (7 - pos));
    }

    void toggle(int offset) 
    {
        int pos = 0;
        unsigned char* start = goto_pos(offset,pos);
        *start ^= 1u << (7 - pos);
    }

    bool check(int offset) 
    {
        int pos = 0;
        unsigned char* start = goto_pos(offset,pos);
        return (*start >> (7 - pos)) & 1u;
    }

    uint16_t to_uint16(int offset,int len)
    {
        assert(len < 16);
        uint16_t number = 0;
        bitwise n((unsigned char*)&number,sizeof(uint16_t));
        for (int i = 0; i < len; i++)
        {
            if (check(i + offset))
            {
                n.set(i + 16 - len);
            }
        }
        std::cout << std::endl;
        reverse_byte((unsigned char*)&number, sizeof(number));
        return number;
    }


    static void swap(unsigned char &left, unsigned char &right);
    static void reverse_byte(unsigned char* data, int len);

private:
    unsigned char* goto_pos(int offset,int &pos)
    {
        unsigned char* start = data_ + offset / 8;
        pos = offset % 8;
        return start;
    }

    unsigned char* data_;
    int len_;
};

void bitwise::swap(unsigned char &left, unsigned char &right)
{
    char temp = left;
    left = right;
    right = temp;
}

void bitwise::reverse_byte(unsigned char* data, int len)
{
   for (int i = 0;i < len /2;i++)
   {
       swap(*(data + i), *(data + len - 1 - i));
   }
}

#endif /* BITWISE_H */
