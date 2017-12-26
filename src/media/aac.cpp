#include "bitwise.h"
#include <gtest/gtest.h>
#include <fstream>


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
        for (;adts();) {}
    }

private:
    bool adts()
    {
        unsigned char header[7] = {0};
        uint16_t len = adts_header();
        if (0 == len)
        {
            return false;
        }
        adts_data(len - 7);
        return true;
    }

    uint16_t adts_header()
    {
        unsigned char header[7] = {0};
        if (!fs_.read((char*)header, 7))
        {
            return 0;
        }
        
        if (!adts_fixed_header(header))
        {
            return 0;
        }
        return adts_variable_header(header);
    }

    bool adts_fixed_header(unsigned char* header)
    {
        return syncword(header);
    }

    uint16_t adts_variable_header(unsigned char* header)
    {
        static int num = 0;
        
        std::cout << num++ << "\t";

        switch (profile(header))
        {
        case 0: std::cout << "Main"; break;
        case 1: std::cout << "LC"; break;
        case 2: std::cout << "SSR"; break;
        default: std::cout << "unkown"; break;
        }

        std::cout << "\t";

        switch (sample_freq_index(header))
        {
        case 0:  std::cout << "96000Hz"; break;
        case 1:  std::cout << "88200Hz"; break;
        case 2:  std::cout << "64000Hz"; break;
        case 3:  std::cout << "48000Hz"; break;
        case 4:  std::cout << "44100Hz"; break;
        case 5:  std::cout << "32000Hz"; break;
        case 6:  std::cout << "24000Hz"; break;
        case 7:  std::cout << "22050Hz"; break;
        case 8:  std::cout << "16000Hz"; break;
        case 9:  std::cout << "12000Hz"; break;
        case 10: std::cout << "11025Hz"; break;
        case 11: std::cout << "8000Hz"; break;
        default: std::cout << "unknown"; break;
        }
        std::cout << "\t";

        uint16_t len = aac_frame_length(header);

        std::cout << len << std::endl;
        return len;
    }

    bool syncword(unsigned char* sync)
    {
        if ((sync[0] & 0xff) == 0xff && (sync[1] & 0xf0) == 0xf0) 
        {
            return true;
        }
        return false;
    }

    uint16_t aac_frame_length(unsigned char* header)
    {
        bitwise b(header, 7);
        return b.to_number<uint16_t>(30, 13);
    }

    uint8_t profile(unsigned char* header) 
    {
        bitwise b(header, 7);
        return b.to_number<uint8_t>(16, 2);
    }

    uint8_t sample_freq_index(unsigned char* header)
    {
        bitwise b(header, 7);
        return b.to_number<uint8_t>(18, 4);
    }


    void adts_data(uint16_t len)
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

