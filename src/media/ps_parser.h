#ifndef PS_PARSER_H
#define PS_PARSER_H
#include "bitstream.h"
#include <assert.h>

/*mpeg-2 programe stream*/

class PSParser 
{
public:
    PSParser(BitStream* input)
        :input_(input)
    {}

    void parse()
    {
        do 
        {
            pack();
        } while (pack_start_code());
    }

private:
    void pack()
    {
        std::cout << std::hex << input_->cur_pos() << "  ";
        pack_header();
        while (!pack_start_code())
        {
            pes_packet();
        }
    }

    void pack_header()
    {
        uint32_t start_code = input_->read_field<uint32_t>(32);
        input_->read_field<uint8_t>(2); // '01'
        input_->read_field<uint8_t>(3); 
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint16_t>(15); 
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint16_t>(15); 
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint16_t>(9); 
        input_->read_field<uint8_t>(1); //marker_bit
        uint32_t mux_rate = input_->read_field<uint32_t>(22);  //programe_mux_rate
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint8_t>(5); //reserved
        uint8_t stuffing_len = input_->read_field<uint8_t>(3); //stuffing_length
        char* stuffing = NULL;
        input_->read_chunk(&stuffing,stuffing_len);
        if (system_header_start_code())
        {
            system_header();
        }
    }


    void system_header()
    {
        input_->read_field<uint32_t>(32); //system header start code
        uint16_t header_len = input_->read_field<uint16_t>(16);
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint32_t>(22); //rate_bound
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint8_t>(6); //audio_bound
        input_->read_field<uint8_t>(1); //fixed_flag
        input_->read_field<uint8_t>(1); //SCPS_flag
        input_->read_field<uint8_t>(1); //system_audio_lock_flag
        input_->read_field<uint8_t>(1); //system_video_lock_flag
        input_->read_field<uint8_t>(1); //marker_bit
        input_->read_field<uint8_t>(5); //video_bound
        input_->read_field<uint8_t>(1); //packet_rate_restriction_flag
        input_->read_field<uint8_t>(7); //reserved_bits

        while (input_->look_ahead<uint8_t>(0,1) == 1)
        {
            uint8_t stream_id = input_->read_field<uint8_t>(8);
            input_->read_field<uint8_t>(2); //'11'
            input_->read_field<uint8_t>(1); //P-STD_buffer_bound_scale
            input_->read_field<uint16_t>(13); //P-STD_buffer_size_bound
        }
    }

    void pes_packet()
    {
        uint32_t packet_start_code_prefix = input_->read_field<uint32_t>(24);
        uint8_t stream_id = input_->read_field<uint8_t>(8);
        uint16_t pes_packet_length = input_->read_field<uint16_t>(16);
        char* data = 0;
        std::cout << "stream id: " << std::hex << (int)stream_id
            << " pes_length:" << std::dec << (int)pes_packet_length << std::endl;

        int len = 0;
        while (len < pes_packet_length)
        {
            int read_len = input_->read_chunk(&data, pes_packet_length - len);
            len += read_len;
        }
    }

    bool pack_start_code()
    {
        uint32_t start_code = input_->look_ahead<uint32_t>(0,32);
        if (0x01BA == start_code)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool packet_start_code_prefix()
    {
        uint32_t prefix = input_->look_ahead<uint32_t>(0,24);
        if (1 == prefix)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool system_header_start_code()
    {
        uint32_t code = input_->look_ahead<uint32_t>(0, 32);
        if (0x01BB == code)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    BitStream* input_;
};
#endif /* PS_PARSER_H */
