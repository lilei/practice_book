#include "bitstream.h"

class BitParser
{
public:
    BitParser()
    {}

    virtual ~BitParser()
    {}

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
        int read_len = 0;
        while (read_len < len)
        {
            read_len += input_->read_chunk(&data, len - read_len);
        }
        return read_len;
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
};

