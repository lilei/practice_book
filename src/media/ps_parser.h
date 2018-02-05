#ifndef PS_PARSER_H
#define PS_PARSER_H
#include "bit_parser.h"
#include <assert.h>

class PSParser :public BitParser
{
public:
    PSParser(char* data, int len)
        :BitParser(data, len)
    {}

    void parse()
    {
        do 
        {
            pack();
        } while (look_ahead<uint32_t>(0,24) == 0x000001);
    }

    void pack()
    {
        pack_header();
        while (look_ahead<uint32_t>(0,24) == 0x000001)
        {
            pes_packet();
        }
    }

    void pack_header()
    {
        uint32_t pack_start_code = read<uint32_t>(32);
        assert(pack_start_code == 0x000001BA);
        assert(read<uint8_t>(2) == 1);
        read<uint8_t>(3);  // system_clock_refernce_base
        read<uint8_t>(1);  //marker_bit
        read<uint8_t>(15); // system_clock_refernce_base
        read<uint8_t>(1);  //marker_bit
        read<uint8_t>(15); // system_clock_refernce_base
        read<uint8_t>(1);  //marker_bit
        read<uint8_t>(9);  // system_clock_refernce_extension
        read<uint8_t>(1);  //marker_bit
        read<uint32_t>(22); //program_mux_rate
        read<uint8_t>(1);  //marker_bit
        read<uint8_t>(1);  //marker_bit
        read<uint8_t>(5);  //reserved
        uint8_t len = read<uint8_t>(3);  //pack_stuffing_length
        for (int i = 0;i < len;i++)
        {
            read<uint8_t>(8);  //stuffing_byte
        }
        if (look_ahead<uint32_t>(0,32) == 1)
        {
            system_header();
        }

    }

    void pes_packet()
    {
        read<uint32_t>(24); //packet_start_code_prefix 0x000001
        read<uint8_t>(8); //sterm_id
        uint16_t len = read<uint16_t>(16); //PES_packet_length
        consume(len * 8);
    }

    void system_header()
    {
        read<uint32_t>(32);  //system_header_start_code
        read<uint16_t>(16);  //header_length
        read<uint8_t>(1);    //marker_bit
        read<uint32_t>(22);  //rate_bound
        read<uint8_t>(1);    //marker_bit
        read<uint32_t>(6);   //audio_bound
        read<uint8_t>(1);    //fixed_flag
        read<uint8_t>(1);    //CSPS_flag
        read<uint8_t>(1);    //system_audio_lock_flag
        read<uint8_t>(1);    //system_video_lock_flag
        read<uint8_t>(1);    //marker_bit
        read<uint32_t>(5);   //video_bound
        read<uint8_t>(1);    //packet_rate_restriction_flag
        while (look_ahead<uint8_t>(0,1) == '1')
        {
            read<uint8_t>(8);   //stream_id
            read<uint8_t>(2);   //'11'
            read<uint8_t>(1);   //P-STD_buffer_bound_scale
            read<uint8_t>(13);  //P-STD_buffer_size_bound
        }
    }
};
#endif /* PS_PARSER_H */
