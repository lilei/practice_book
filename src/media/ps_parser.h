#ifndef PS_PARSER_H
#define PS_PARSER_H
#include "bit_parser.h"
#include <functional>

/* 
    iso13812-1
    mpeg-2 program stream
*/

class PSParser :public BitParser
{
public:
    /*the entrance*/
    void parse(BitStream& input);

    /*callback functions*/
    std::function<void(uint32_t)> on_timestamp;
    std::function<void(uint8_t,uint8_t)> on_stream_type;
    std::function<void(char*, int)> on_video_es;
    std::function<void(char*, int)> on_audio_es;
    std::function<void()> on_pack_end;


private:
    /*parsing tree*/
    void pack();
        void pack_header();
            void system_header();
            void pes_packet();
                void pes_es(uint8_t stream_id,uint16_t pes_packet_length);
                void program_stream_map(uint8_t stream_id,uint16_t pes_packet_length);
                    int descriptor();
                        void video_descriptor();
                        void audio_descriptor();


    /*lookahead functions*/
    bool pack_start_code();
    bool packet_start_code_prefix();
    bool system_header_start_code();
};
#endif /* PS_PARSER_H */
