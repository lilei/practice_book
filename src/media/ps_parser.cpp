#include "ps_parser.h"
#include <assert.h>

void PSParser::parse(BitStream& input)
{
    input_ = &input;
    while (!pack_start_code())
    {
        discard_chunk(1);
    }
    while (pack_start_code())
    {
        pack();
    }
}

void  PSParser::pack()
{
    pack_header();
    while (!pack_start_code())
    {
        pes_packet();
    }
    on_pack_end();
}

void PSParser::pack_header()
{
    uint32_t start_code = read_field<uint32_t>(32);
    read_field<uint8_t>(2); // '01'
    read_field<uint8_t>(3);
    read_field<uint8_t>(1); //marker_bit
    read_field<uint16_t>(15);
    read_field<uint8_t>(1); //marker_bit
    read_field<uint16_t>(15);
    read_field<uint8_t>(1); //marker_bit
    read_field<uint16_t>(9);
    read_field<uint8_t>(1); //marker_bit
    uint32_t mux_rate = read_field<uint32_t>(22);  //programe_mux_rate
    read_field<uint8_t>(1); //marker_bit
    read_field<uint8_t>(1); //marker_bit
    read_field<uint8_t>(5); //reserved
    uint8_t stuffing_len = read_field<uint8_t>(3); //stuffing_length
    discard_chunk(stuffing_len);
    if (system_header_start_code())
    {
        system_header();
    }
}


void PSParser::system_header()
{
    read_field<uint32_t>(32); //system header start code
    uint16_t header_len = read_field<uint16_t>(16);
    read_field<uint8_t>(1); //marker_bit
    read_field<uint32_t>(22); //rate_bound
    read_field<uint8_t>(1); //marker_bit
    read_field<uint8_t>(6); //audio_bound
    read_field<uint8_t>(1); //fixed_flag
    read_field<uint8_t>(1); //SCPS_flag
    read_field<uint8_t>(1); //system_audio_lock_flag
    read_field<uint8_t>(1); //system_video_lock_flag
    read_field<uint8_t>(1); //marker_bit
    read_field<uint8_t>(5); //video_bound
    read_field<uint8_t>(1); //packet_rate_restriction_flag
    read_field<uint8_t>(7); //reserved_bits

    while (look_ahead<uint8_t>(0, 1) == 1)
    {
        uint8_t stream_id = read_field<uint8_t>(8);
        read_field<uint8_t>(2); //'11'
        read_field<uint8_t>(1); //P-STD_buffer_bound_scale
        read_field<uint16_t>(13); //P-STD_buffer_size_bound
    }
}


void PSParser::pes_packet()
{
    uint32_t packet_start_code_prefix = read_field<uint32_t>(24);
    uint8_t stream_id = read_field<uint8_t>(8);
    uint16_t pes_packet_length = read_field<uint16_t>(16);

    //std::cout << " stream_id: " << std::hex << (int)stream_id <<" length: " << std::dec << pes_packet_length << std::endl;
    if (0xBC != stream_id && //program_stream_map
        0xBE != stream_id && //padding_stream
        0xBF != stream_id && //private_stream_2
        0xF0 != stream_id && //ECM
        0xF1 != stream_id && //EMM
        0xF2 != stream_id && //DSMCC_stream
        0xFF != stream_id && //program_stream_directory
        0xF8 != stream_id)   //ITU-T Rec.H.222.1 type E stream
    {
        pes_es(stream_id,pes_packet_length);
    }
    else if (0xBC == stream_id)
    {
        program_stream_map(stream_id,pes_packet_length);
    }
    else
    {
        discard_chunk(pes_packet_length);
    }
}

void PSParser::pes_es(uint8_t stream_id,uint16_t pes_packet_length)
{
    read_field<uint8_t>(2);                              //10
    read_field<uint8_t>(2);                              //PES_scrambling_control
    read_field<uint8_t>(1);                              //PES_priority
    read_field<uint8_t>(1);                              //data_alignment_indicator
    read_field<uint8_t>(1);                              //copyright
    read_field<uint8_t>(1);                              //orignal_or_copy 
    uint8_t pts_dts_flags = read_field<uint8_t>(2);      //PTS_DTS_flags
    read_field<uint8_t>(1);                              //ESCR_flag
    read_field<uint8_t>(1);                              //ES_rate_flag
    read_field<uint8_t>(1);                              //DSM_trick_mode_flag
    read_field<uint8_t>(1);                              //additional_copy_info_flag
    read_field<uint8_t>(1);                              //PES_CRC_flag
    read_field<uint8_t>(1);                              //PES_extension_flag
    uint8_t pes_header_len = read_field<uint8_t>(8);     //PES_header_data_length

    if (2 == pts_dts_flags)
    {
        read_field<uint8_t>(4);                            //0010
        uint32_t pts1 = read_field<uint32_t>(3);           //PTS[32..30]
        read_field<uint8_t>(1);                            //marker_bit
        uint32_t pts2 = read_field<uint32_t>(15);          //PTS[29..15]
        read_field<uint8_t>(1);                            //marker_bit
        uint32_t pts3 = input_->read_field<uint32_t>(15);  //PTS[14..0]
        read_field<uint8_t>(1);                            //marker_bit

        uint32_t timestamp = (pts1 << 30) | (pts2 << 15) | pts3;
        if (stream_id >= 0xE0 && stream_id <= 0xEF)
        {
            on_timestamp(timestamp);
        }
        discard_chunk(pes_header_len - 5);
    }
    else if (3 == pts_dts_flags)
    {
        read_field<uint8_t>(4);   //0011
        read_field<uint8_t>(3);   //PTS[32..30]
        read_field<uint8_t>(1);   //marker_bit
        read_field<uint16_t>(15); //PTS[29..15]
        read_field<uint8_t>(1);   //marker_bit
        read_field<uint16_t>(15);  //PTS[14..0]
        read_field<uint8_t>(1);   //marker_bit

        read_field<uint8_t>(4);   //0001
        read_field<uint8_t>(3);   //PTS[32..30]
        read_field<uint8_t>(1);   //marker_bit
        read_field<uint16_t>(15); //PTS[29..15]
        read_field<uint8_t>(1);   //marker_bit
        read_field<uint16_t>(15);  //PTS[14..0]
        read_field<uint8_t>(1);   //marker_bit
        discard_chunk(pes_header_len - 10);
    }
    else
    {
        discard_chunk(pes_header_len);
    }
    if (stream_id >= 0xE0 && stream_id <= 0xEF)
    {
        read_chunk(pes_packet_length - 3 - pes_header_len, on_video_es);
    }
    else
    {
        discard_chunk(pes_packet_length - 3 - pes_header_len);
    }
}


void PSParser::program_stream_map(uint8_t stream_id,uint16_t pes_packet_length)
{
    uint8_t indicator = read_field<uint8_t>(1);         //current_next_indicator 
    read_field<uint8_t>(2);                             //reserved
    read_field<uint8_t>(5);                             //program_stream_map_version
    read_field<uint8_t>(7);                             //reserved
    read_field<uint8_t>(1);                             //marker_bit
    uint16_t ps_info_length = read_field<uint16_t>(16); //program_stream_info_length
    int len = 0;
    while (len < ps_info_length)
    {
        len += descriptor();
    }
    uint16_t es_map_length = read_field<uint16_t>(16); //elementary_stream_map_length

    len = 0;
    while (len < es_map_length)
    {
        uint8_t stream_type = read_field<uint8_t>(8);   //stream_type
        uint8_t elementary_stream_pid = read_field<uint8_t>(8);   //elementary_stream_pid
        on_stream_type(elementary_stream_pid,stream_type);


        uint16_t es_info_length = read_field<uint16_t>(16); //elementary_stram_info_length
        len += es_info_length + 4;

        int len1 = 0;
        while (len1 < es_info_length)
        {
            len1 += descriptor();
        }
    }
    read_field<uint32_t>(32); //CRC_32
}

int PSParser::descriptor()
{
    uint8_t descriptor_tag = read_field<uint8_t>(8);
    uint8_t descriptor_length = read_field<uint8_t>(8);
    //std::cout << "descriptor tag: " << (int)descriptor_tag << "  descriptor length: " << (int)descriptor_length<<std::endl;
    if (2 == descriptor_tag)
    {
        video_descriptor();
    }
    else if (3 == descriptor_tag)
    {
        audio_descriptor();
    }
    else
    {
        //unknown stream sescriptor
        discard_chunk(descriptor_length);
    }
    return descriptor_length + 2;
}

void PSParser::video_descriptor()
{
    read_field<uint8_t>(1); //multiple_frame_rate_flag
    read_field<uint8_t>(4); //frame_rate_code
    uint8_t MPEG_1_only_flag = read_field<uint8_t>(1);
    read_field<uint8_t>(1); //constrained_parameter_flag
    read_field<uint8_t>(1); //still_picture_flag
    if (0 == MPEG_1_only_flag)
    {
        read_field<uint8_t>(8); //profile_and_level_indication
        read_field<uint8_t>(2); //chroma_format
        read_field<uint8_t>(1); //frame_rate_extension_flag
        read_field<uint8_t>(5); //reserved
    }
}

void PSParser::audio_descriptor()
{
    read_field<uint8_t>(1); //free_format_flag
    read_field<uint8_t>(1); //ID
    read_field<uint8_t>(2); //layer
    read_field<uint8_t>(1); //variable_rate_audio_indicator
    read_field<uint8_t>(3); //reserved
}

bool PSParser::pack_start_code()
{
    uint32_t start_code = look_ahead<uint32_t>(0, 32);
    if (0x01BA == start_code)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PSParser::packet_start_code_prefix()
{
    uint32_t prefix = look_ahead<uint32_t>(0, 24);
    if (1 == prefix)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PSParser::system_header_start_code()
{
    uint32_t code = look_ahead<uint32_t>(0, 32);
    if (0x01BB == code)
    {
        return true;
    }
    else
    {
        return false;
    }
}
