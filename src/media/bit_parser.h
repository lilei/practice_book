#ifndef BIT_PARSER_H
#define BIT_PARSER_H

#include "bitwise.h"
#include <string>
#include <stdint.h>
#include <assert.h>

class BitParser
{
public:
    BitParser(char* data,int octc_len)
        :bits_(data,octc_len),offset_(0)
    {
    }

    template<typename T>
    T next_filed(int bit_len)
    {
        T value = bits_.to_number<T>(offset_,bit_len);
        offset_ += bit_len;
        return value;
    }


    template<typename T>
    T look_ahead(int start,int bit_len)
    {
        T value = bits_.to_number<T>(start,bit_len);
        return value;
    }

    template<typename T>
    T read(int bit_len)
    {
        T value = bits_.to_number<T>(offset_,bit_len);
        consume(bit_len);
        return value;
    }

    BitParser& consume(int bit_len)
    {
        offset_ += bit_len;
        return *this;
    }

    bool eof()
    {
        return offset_ >= bits_.size() * 8;
    }

private:
    bitwise bits_;
    int offset_;
};

#endif /* BIT_PARSER_H */
