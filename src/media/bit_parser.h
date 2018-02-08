#include "bitstream.h"
#include <functional>

class BitParser
{
public:
    BitParser()
    {}

    virtual ~BitParser()
    {}

public:
    void data_callback(std::function<void(char*,int)> &func)
    {
        func_data_ = func;
    }

protected:
    template<typename T>
    T look_ahead(int start, int bit_len)
    {
        return input_->look_ahead<T>(start, bit_len);
    }

    template<typename T>
    T read_field(int bit_len)
    {
        return input_->read_field<T>(bit_len);
    }

    int read_chunk(int len)
    {
        char* data = NULL;
        int total_len = 0;
        while (total_len < len)
        {
            
            int read_len = input_->read_chunk(&data, len - total_len);
            func_data_(data,read_len);
            total_len += read_len;
        }
        return total_len;
    }

    void discard_chunk(int len)
    {
        char* data = NULL;
        int read_len = 0;
        while (read_len < len)
        {
            read_len += input_->read_chunk(&data, len - read_len);
        }
    }

    BitStream* input_;
    std::function<void(char*,int)> func_data_;
};

