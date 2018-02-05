#ifndef RTP_H265_H
#define RTP_H265_H

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
namespace h265
{
    struct NaluHeader 
    {
        uint16_t forbidden_zero_bit  : 1;
        uint16_t nal_unit_type       : 6;
        uint16_t nuh_layer_id        : 6;
        uint16_t nuh_tid             : 3;
    };


    struct fuHeader
    {
        uint8_t fu_s      : 1;
        uint8_t fu_e      : 1;
        uint8_t nalu_type : 6; 
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
            uint8_t packet_type = input_->look_ahead<uint8_t>(1, 6);
            if (packet_type < 41)
            {
                nalu(0);
            }
            else if (48 == packet_type)
            {
                aps();
            }
            else if (49 == packet_type)
            {
                fu();
            }
            else if (50 == packet_type)
            {
                paci();
            }
        }

    private:
        void nalu_header(NaluHeader& header)
        {
            header.forbidden_zero_bit = input_->read_field<uint8_t>(1);
            header.nal_unit_type      = input_->read_field<uint8_t>(6);
            header.nuh_layer_id       = input_->read_field<uint8_t>(6);
            header.nuh_tid            = input_->read_field<uint8_t>(3);
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
                input_->read_chunk(&data,size - 2);
            }
            return true;
        }

        void aps()
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

        bool fu()
        {
            NaluHeader nalu_hdr = {0};
            nalu_header(nalu_hdr);

            fuHeader header = {0};
            header.fu_s      = input_->read_field<uint8_t>(1);
            header.fu_e      = input_->read_field<uint8_t>(1);
            header.nalu_type = input_->read_field<uint8_t>(6);
            std::cout << (int)header.nalu_type << std::endl;
            char* data = NULL;
            input_->read_chunk(&data, BitStream::END_POS);
            return true;
        }

        void paci()
        {
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

};/*end of namespace h265*/

}; /*end of namespace rtp*/

#endif /* NALU_H */
