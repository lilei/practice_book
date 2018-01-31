#ifndef RTP_H264_H
#define RTP_H264_H

#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include "bit_parser.h"

/*
    H.264/RTP (RFC 6184)
*/

namespace rtp
{
namespace h264
{
    struct NaluHeader
    {
        uint8_t forbidden_zero_bit : 1;
        uint8_t nal_ref_idc        : 2;
        uint8_t nal_unit_type      : 5;
    };

    struct fuHeader
    {
        uint8_t fu_s      : 1;
        uint8_t fu_e      : 1;
        uint8_t fu_r      : 1;
        uint8_t nalu_type : 5;
    };

    class RtpPayload :public BitParser
    {
    public:
        RtpPayload (char* data,int len) 
            :BitParser(data,len)
        {
            parse();
        }

    private:
        void parse()
        {
            uint8_t packet_type = look_ahead<uint8_t>(3, 5);
            if (packet_type < 24 && packet_type > 0)
            {
                nalu(0);
            }
            else if (24 == packet_type)
            {
                stap_a();
            }
            else if (28 == packet_type)
            {
                fuHeader header;
                fu(header);
            }
        }

        bool nalu_header(NaluHeader& header)
        {
            if (eof())
            {
                return false;
            }

            header.forbidden_zero_bit = read<uint8_t>(1);
            header.nal_ref_idc        = read<uint8_t>(2);
            header.nal_unit_type      = read<uint8_t>(5);
            return true;
        }

        bool nalu(int size)
        {
            NaluHeader header = {0};
            if (!nalu_header(header))
            {
                return false;
            }
            std::cout << (int)header.nal_unit_type << std::endl;
            consume(size * 8 - 8);
            return true;
        }

        void stap_a()
        {
            NaluHeader header = {0};
            nalu_header(header);
            uint16_t size = 0;
            while (nalu_size(size))
            {
                nalu(size);
            }
        }

        bool fu(fuHeader& header)
        {
            NaluHeader nalu_hdr = {0};
            nalu_header(nalu_hdr);
            header.fu_s      = read<uint8_t>(1);
            header.fu_e      = read<uint8_t>(1);
            header.fu_r      = read<uint8_t>(1);
            header.nalu_type = read<uint8_t>(5);
            std::cout << (int)header.nalu_type << std::endl;
            return true;
        }

        bool nalu_size(uint16_t& len)
        {
            if (eof())
            {
                return false;
            }
            len = read<uint16_t>(16);

            //padding
            if (0 == len)
            {
                return false;
            }

            return true;
        }

    };

};/*end of namespace h264*/

}; /*end of namespace rtp*/

#endif /* NALU_H */
