#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "bitwise.h"
#include <stdint.h>
#include <assert.h>
#include <iostream>
#include <vector>

class BitStreamError
{
};

class BitStream
{
public:
    BitStream(int size,int k)
        :offset_(0),read_(NULL)
    {
        for (int i = 0;i < size + k;i++)
        {
            data_.push_back(0);
        }
        write_ = data_.data();
        read_ = data_.data();
    }

    virtual ~BitStream()
    {}

    char* start()
    {
        return write_;
    }

    virtual int on_write(char* buff, int size) = 0;

    template<typename T>
    T look_ahead(int start,int bit_len)
    {
        if (overflow(bit_len))
        {
            back_to_front();
            fetch();
        }
        bitwise bits(read_,64);
        return bits.to_number<T>(offset_ + start,bit_len);
    }

    template<typename T>
    T read_field(int bit_len)
    {
        if (overflow(bit_len))
        {
            back_to_front();
            fetch();
        }
        T number = look_ahead<T>(0,bit_len);
        consume(bit_len);
        return number;
    }

    static const int END_POS = -1;
    int read_chunk(char**data,int len)
    {
        //byte alignling
        assert(offset_ % 8 == 0);
        if (END_POS == len)
        {
            consume(rest_size() * 8);
            return rest_size();
        }
        if (overflow(len * 8))
        {
            back_to_front();
            fetch();
        }
        *data = read_;
        int read_len = 0;
        if (rest_size() >= len)
        {
            read_len = len;
        }
        else
        {
            read_len = rest_size();
        }
        consume(read_len * 8);
        return read_len;
    }

    bool eof()
    {
        return write_ == read_;
    }

private:
    void consume(int bit_len)
    {
        offset_ += bit_len;
        read_ += offset_ / 8;
        offset_ = offset_ % 8;
    }
    
    const char* start_pos() const
    {
        return data_.data();
    }

    const char* end_pos() const
    {
        return data_.data() + data_.size();
    }

    int rest_size() const
    {
        return  end_pos() - read_;
    }

    bool overflow(int bit_len) const
    {
        if ((write_ - read_) * 8 - offset_ < bit_len)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //move back bytes to front,to make a continuely chunk to write
    void back_to_front()
    {
        if (write_ == start_pos())
        {
            return;
        }
        const char* last = write_;
        write_ = (char*)start_pos();
        for (;read_ < last;read_++,write_++)
        {
            *write_ = *read_;
        }
        read_ = (char*)start_pos();
    }

    void fetch()
    {
        int len = on_write(write_, rest_size());
        if (len < 0)
        {
            throw BitStreamError();
        }
        else
        {
            write_ += len;
        }
    }

    std::vector<char> data_;
    char* read_;
    char* write_;
    int offset_;
};

#endif /* BITSTREAM_H */
