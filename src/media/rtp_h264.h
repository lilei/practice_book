#ifndef RTP_H264_H
#define RTP_H264_H

#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include "bitstream.h"
#include "rtp.h"

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

    class Packet 
    {
    public:
        Packet (BitStream* input) 
            :input_(input)
        {
        }

        void parse()
        {
            RtpHeader rtp_header(input_);
            rtp_header.parse();
            uint8_t packet_type = input_->look_ahead<uint8_t>(3, 5);
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

    private:
        void nalu_header(NaluHeader& header)
        {
            header.forbidden_zero_bit = input_->read_field<uint8_t>(1);
            header.nal_ref_idc        = input_->read_field<uint8_t>(2);
            header.nal_unit_type      = input_->read_field<uint8_t>(5);
        }

        bool nalu(int size)
        {
            NaluHeader header = {0};
            nalu_header(header);

            std::cout << (int)header.nal_unit_type << std::endl;

            char* data = NULL;
            if (0 == size)
            {
                input_->read_chunk(&data,BitStream::END_POS);
            }
            else
            {
                input_->read_chunk(&data,size - 1);
            }
            return true;
        }

        void stap_a()
        {
            NaluHeader header = {0};
            nalu_header(header);
            while (!input_->eof())
            {
                uint16_t size = 0;
                nalu_size(size);

                //padding
                if (0 == size)
                {
                    break;
                }
                nalu(size);
            }
        }

        bool fu(fuHeader& header)
        {
            NaluHeader nalu_hdr = {0};
            nalu_header(nalu_hdr);
            header.fu_s      = input_->read_field<uint8_t>(1);
            header.fu_e      = input_->read_field<uint8_t>(1);
            header.fu_r      = input_->read_field<uint8_t>(1);
            header.nalu_type = input_->read_field<uint8_t>(5);
            std::cout << (int)header.nalu_type << std::endl;
            char* data = NULL;
            input_->read_chunk(&data, BitStream::END_POS);
            return true;
        }

        bool nalu_size(uint16_t& len)
        {
            len = input_->read_field<uint16_t>(16);

            //padding
            if (0 == len)
            {
                return false;
            }
            return true;
        }

    private:
        BitStream* input_;
    };

};/*end of namespace h264*/

}; /*end of namespace rtp*/

#endif /* NALU_H */
