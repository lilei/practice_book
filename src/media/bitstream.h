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


/*
    the bitstream class is used for parsing binary protocol
    the underline of bitstream can be file or network
    the whole range is [begin,end)
    the readable range is [read_,write_)
    the writable range is [write_,end)
*/
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

    char* begin()
    {
        return data_.data();
    }

    /*the last pos plus 1*/
    char* end()
    {
        return data_.data() + data_.size() + 1;
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
        assert(bit_len <= sizeof(T) * 8);
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

        //reach the end,fetching some new data firstly
        if (eof() && END_POS != len)
        {
            read_ = begin();
            write_ = begin();
            fetch();
        }

        int read_len = 0;
        //read to end
        if (END_POS == len || write_ - read_ < len)
        {
            read_len = write_ - read_;
        }
        else
        {
            read_len = len;
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

    //move back bytes to front,to make a continuous chunk to write
    void back_to_front()
    {
        if (write_ == begin())
        {
            return;
        }
        int rest = write_ - read_;
        for (int i = 0;i < rest;i++)
        {
            *(begin() + i) = *(read_ + i);
        }
        read_ = begin();
        write_ = begin() + rest;
    }

    void fetch()
    {
        int len = on_write(write_, end() - write_ - 1);
        if (len < 0)
        {
            BitStreamError error;
            throw error;
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
