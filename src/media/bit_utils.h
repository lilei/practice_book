#ifndef BIT_UTILS_H
#define BIT_UTILS_H
#include <stdint.h>
#include <vector>

template<typename T>
T byte_to_number(char* data,int len)
{
    assert(sizeof(T) >= len);

    reverse_byte(data, len);

    return *reinterpret_cast<T*>(data);
}

std::vector<bool> extract_bits(unsigned char* start,uint8_t offset,uint8_t bit_len);

uint32_t buffer_to_int32(unsigned char* data, uint8_t offset,uint8_t bit_len);


void swap(char &left,char &right);
void reverse_byte(char* data, int len);

#endif /* BIT_UTILS_H */
