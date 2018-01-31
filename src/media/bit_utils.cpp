#include "bit_utils.h"
#include <stdarg.h>
#include <iostream>
#include <bitset>
#include <assert.h>

void swap(char &left,char &right)
{
    char temp = left;
    left = right;
    right = temp;
}

//·­×ª×Ö½Ú
void reverse_byte(char* data,int len)
{
   for (int i = 0;i < len /2;i++)
   {
       swap(*(data + i), *(data + len - 1 - i));
   }
}

void extract_bits(unsigned char octa, uint8_t offset,uint8_t bit_len,std::vector<bool>& bits)
{
    assert(offset + bit_len <= 8);
    for (int i = 0;i < bit_len;i++)
    {
        bits.push_back((octa >> (7 - i - offset)) & 1u);
    }
}

std::vector<bool> extract_bits(unsigned char* start, uint8_t offset, uint8_t bit_len)
{
    std::vector<bool> bits;
    int got_bits = 0;

    //the pre byte
    while (offset > 8)
    {
        offset -= 8;
        ++start;
    }

    //the head byte
    int read_len = bit_len > 8 - offset ? 8 - offset : bit_len;
    extract_bits(*start,offset,read_len,bits);
    got_bits += read_len;
    ++start;

    //the following bytes
    for (;got_bits < bit_len;)
    {
        read_len = bit_len - got_bits > 8 ? 8 : bit_len - got_bits;
        extract_bits(*start,0,read_len,bits);
        got_bits += read_len;
        ++start;
    }

    return bits;
}


int32_t buffer_to_int32(char*data, int start, int section, ...)
{
    /*
    std::bitset<32> bits;

    int32_t ret = 0;
    va_list args;
    va_start(args,start);
    for (int i = 0;i < section;i++)
    {
        int offset = va_arg(args,int);
        //data << (start + offset);
        data + start + offset;
        //std::cout << a << std::endl;
    }
    va_end(args);
    */
    return 0;
}

uint32_t buffer_to_int32(unsigned char*data, uint8_t offset,uint8_t bit_len)
{
    /*
    assert(bit_len < 32);
    uint32_t n = 0;
    for (int i = 0;i < 32 - bit_len;i++)
    {
        //n |= 
    }

    std::vector<bool> v = extract_bits(data, offset, bit_len);
    for (size_t i = 0;i < v.size();i++)
    {
        bits[i + 32 - bit_len] = v[i];
    }
    return bits.to_ulong();
    */
    return 0;
}

