#include <gtest/gtest.h>
#include <fstream>
#include <array>

struct AdtsVariableHeader
{

};

class AAC
{
public:
    AAC(std::fstream& fs)
        :fs_(fs)
    {}

    void parse()
    {
        while (fs_)
        {
            adts();
        }
    }

private:
    void adts()
    {
        if (!syncword())
        {
            return;
        }
        while (!syncword())
        {
        }
        return;
    }

    bool syncword()
    {
        std::array<uint8_t, 2> temp;
        if (!fs_.read((char*)temp.data(), sizeof(uint16_t)))
        {
            return false;
        }

        if ((temp[0] & 0xff) == 0xff && (temp[1] & 0xf0) == 0xf0) 
        {
            fs_.seekg(-2,std::ios_base::cur);
            return true;
        }
        return false;
    }

    struct AdtsFixedHeader 
    {
        uint16_t syncword                 : 12;
        uint16_t id                       : 1;
        uint16_t layer                    : 2;
        uint16_t protection_absent        : 1;

        uint16_t profile                  : 2;
        uint16_t sampling_frequency_index : 4;
        uint16_t private_bit              : 1;
        uint16_t channel_configuration    : 3;
        uint16_t original_copy            : 1;
        uint16_t home                     : 1;
        uint16_t emphasis                 : 2;
    };

    bool adts_fixed_header()
    {
        std::array<char,16> temp;
        if (!fs_.read(temp.data(), 16));
        {
            return false;
        }


        //syncword 12bit
        if (!fs_.read(temp.data(),12))
        {
            return false;
        }
        
        //id 1bit
        if (!fs_.read(temp.data(),1))
        {
            return false;
        }
        
        //layer 2bit
        if (!fs_.read(temp.data(),2))
        {
            return false;
        }
        
        //protection absent 1bit
        if (!fs_.read(temp.data(),1))
        {
            return false;
        }
        
        //profile 2bit
        if (!fs_.read(temp.data(),2))
        {
            return false;
        }
        
        //sampling frequency index 4bit
        if (!fs_.read(temp.data(),4))
        {
            return false;
        }
        
        //private bit 1bit
        if (!fs_.read(temp.data(),1))
        {
            return false;
        }
        
        //channel configuration 3bit
        if (!fs_.read(temp.data(),3))
        {
            return false;
        }
        
        //original/copy 1bit
        if (!fs_.read(temp.data(),1))
        {
            return false;
        }

        //home 1bit
        if (!fs_.read(temp.data(),1))
        {
            return false;
        }
        
        //emphasis 2bit
        if (!fs_.read(temp.data(),2))
        {
            return false;
        }
    }

    int adts_variable_header()
    {
        std::array<char,16> temp;

        //copyright identification bit 1bit
        if (!fs_.read(temp.data(),1))
        {
            return -1;
        }

        //copyright identification start 1bit
        if (!fs_.read(temp.data(),1))
        {
            return -1;
        }

        //aac frame length 13bit
        if (!fs_.read(temp.data(),13))
        {
            return -1;
        }
        uint32_t len = reverse_bytes(temp.data(),);

        //adts buffer fullness 11bit
        if (!fs_.read(temp.data(),11))
        {
            return -1;
        }

        //no raw data blocks in frame 2bit
        if (!fs_.read(temp.data(),2))
        {
            return -1;
        }

        return len;
    }

    bool adts_data(size_t len)
    {
        fs_.seekg(len,std::ios_base::cur);
    }

    uint32_t reverse_bytes(unsigned char *temp, int size)
    {
        assert(size <= 4);
        uint32_t r = 0;
        for (int i = 0; i < size; i++)
        {
            int x = temp[i];
            x = x << ((size - 1 - i) * 8);
            r |= x;
        }
        return r;
    }

    std::fstream& fs_;
};

TEST(aac,parse)
{
    std::fstream fs("../resource/nocturne.aac", std::ios_base::binary | std::ios_base::in);
    if (fs)
    {
        AAC aac(fs);
        aac.parse();
    }
}

