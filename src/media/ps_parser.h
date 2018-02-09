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
    void parse(BitStream& input);

    std::function<void(uint32_t)> on_timestamp;

    std::function<void(char*, int)> on_video_es;

    std::function<void(char*, int)> on_audio_es;

private:
    void pack();

    void pack_header();

    void system_header();

    void pes_packet();

    void system_map();

    void descriptor();

    void video_stream_descriptor();

    void audio_stream_descriptor();

    bool pack_start_code();

    bool packet_start_code_prefix();

    bool system_header_start_code();
};
#endif /* PS_PARSER_H */
