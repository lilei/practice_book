#ifndef RTP_H265_H
#define RTP_H265_H

#include "bit_parser.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>

/*
    HEVC/RTP (RFC 7798)
*/

namespace rtp
{
namespace h265
{
    struct NaluHeader 
    {
        uint16_t nuh_tid             : 3;
        uint16_t nuh_layer_id        : 6;
        uint16_t nal_unit_type       : 6;
        uint16_t forbidden_zero_bit  : 1;
    };


    struct fuHeader
    {
        uint8_t nalu_type : 6; 
        uint8_t fu_e      : 1;
        uint8_t fu_s      : 1;
    };

    struct Fu
    {
        fuHeader fu_header;
        char*    fu_data;
        uint16_t fu_len; /*include the header len*/
    };

    class RtpPayload :public BitParser
    {
    public:
        RtpPayload (char* data,int len) 
            :BitParser(data,len)
        {
            parse();
        }

        int packet_type() const
        {
            return packet_type_;
        }

    private:
        void parse()
        {
            NaluHeader header = {0};
            nalu_header(header);
            if (header.nal_unit_type < 41)
            {
                back(16);
                NaluHeader header = {0};
                nalu(header,0);
            }
            else if (48 == header.nal_unit_type)
            {
                aps();
            }
            else if (49 == header.nal_unit_type)
            {
                fuHeader header;
                fu(header);
            }
            else if (50 == header.nal_unit_type)
            {
                paci();
            }
        }

        bool nalu_header(NaluHeader& header)
        {
            if (eof())
            {
                return false;
            }

            header.forbidden_zero_bit = get<uint8_t>(1);
            header.nal_unit_type      = get<uint8_t>(6);
            header.nuh_layer_id       = get<uint8_t>(6);
            header.nuh_tid            = get<uint8_t>(3);
            return true;
        }

        bool nalu(NaluHeader& header,int size)
        {
            if (!nalu_header(header))
            {
                return false;
            }
            std::cout << (int)header.nal_unit_type << std::endl;
            consume(size * 8 - 16);
            return true;
        }

        void aps()
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
            header.nalu_type = get<uint8_t>(6);
            std::cout << (int)header.nalu_type << std::endl;
            return true;
        }

        bool paci()
        {
            return true;
        }

        /*

        BitParser donl(char* data)
        {
            BitParser parser(data,2);
            parser[16];
            return parser;
        }
        */

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

        int packet_type_;
        char* data_;
        int len_;
        bool important_;
        
    };
};/*end of namespace h265*/

}; /*end of namespace rtp*/

#endif /* RTP_H265_H */
