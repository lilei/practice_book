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
            NaluHeader header = {0};
            nalu_header(header);
            if (header.nal_unit_type < 24 && header.nal_unit_type > 0)
            {
                back(8);
                nalu(header,0);
            }
            else if (24 == header.nal_unit_type)
            {
                stap_a();
            }
            else if (28 == header.nal_unit_type)
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

            header.forbidden_zero_bit = get<uint8_t>(1);
            header.nal_ref_idc        = get<uint8_t>(2);
            header.nal_unit_type      = get<uint8_t>(5);
            return true;
        }

        bool nalu(NaluHeader& header,int size)
        {
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
            uint16_t size = 0;
            while (nalu_size(size))
            {
                NaluHeader header = {0};
                nalu(header, size);
            }
        }

        bool fu(fuHeader& header)
        {
            if (eof())
            {
                return false;
            }
            header.fu_s      = get<uint8_t>(1);
            header.fu_e      = get<uint8_t>(1);
            header.fu_r      = get<uint8_t>(1);
            header.nalu_type = get<uint8_t>(5);
            std::cout << (int)header.nalu_type << std::endl;
            return true;
        }

        bool nalu_size(uint16_t& len)
        {
            if (eof())
            {
                return false;
            }
            len = get<uint16_t>(16);

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
