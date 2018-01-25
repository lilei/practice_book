#ifndef NALU_H
#define NALU_H
#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include "bitwise.h"

/*
    H.264 / RTP (RFC 6184)
    HEVC  / RTP (RFC 7798)
*/

namespace rtp
{
namespace h264
{
    struct NaluHeader
    {
        uint8_t nal_unit_type      : 5;
        uint8_t nal_ref_idc        : 2;
        uint8_t forbidden_zero_bit : 1;
    };

    struct fuHeader
    {
        uint8_t nalu_type : 5;
        uint8_t fu_r      : 1;
        uint8_t fu_e      : 1;
        uint8_t fu_s      : 1;
    };

    class RtpPayload
    {
    public:
        RtpPayload(char* data,int len) 
            :data_(data),
            len_(len),
            packet_type_(0),
            important_(false)
        {
            parse();
        }

        int packet_type() const
        {
            return packet_type_;
        }

        /*
            sps pps sei idr is important
            non-idr is unimportant
        */
        bool important() const
        {
            return important_;
        }

    private:
        void parse()
        {
            h264::NaluHeader* header = (h264::NaluHeader*)(data_);
            packet_type_ = header->nal_unit_type;

            /*single nalu*/
            if (packet_type_ > 0 && packet_type_ < 24)
            {
                nalu((NaluHeader*)data_);
            }
            /*STAP-A*/
            else if (24 == packet_type_)
            {
                stap_a();
            }
            /*FU-A*/
            else if (28 == packet_type_)
            {
                fu((fuHeader*)(data_ + 1));
            }
            else
            {
                std::cout << "unkonwn packet type" << packet_type_ << std::endl;
            }
        }

        /*return the nal uint type*/
        int nalu(NaluHeader* header)
        {
            std::cout << (int)(header->nal_unit_type) << std::endl;
            important(header->nal_unit_type);
            return header->nal_unit_type;
        }

        /*STAP-A: singile-time aggregation packet */
        void stap_a()
        {
            int offset = 1;
            do
            {
                uint16_t nalu_len = *(uint16_t*)(data_ + offset);
                bitwise::reverse_byte((unsigned char*)&nalu_len, 2);
                //padding
                if (0 == nalu_len)
                {
                    break;
                }

                nalu((NaluHeader*)(data_ + offset + 2));
                offset += (2 + nalu_len);
                if (offset == len_)
                {
                    break;
                }
            } while (true);
        }

        /*return the nal uint type*/
        int fu(fuHeader *header)
        {
            std::cout << (int)(header->nalu_type) << std::endl;
            important(header->nalu_type);
            return header->nalu_type;
        }

        void important(int nalu_type)
        {
            if (5 == nalu_type || /*idr*/
                6 == nalu_type || /*sei*/
                7 == nalu_type || /*sps*/
                8 == nalu_type)   /*pps*/
            {
                if (!important_)
                {
                    important_ = true;
                }
            }
        }

        int packet_type_;
        char* data_;
        int len_;
        bool important_;
        
    };

};/*end of namespace h264*/

namespace h265
{
    struct NaluHeader 
    {
        uint16_t nuh_tid             : 3;
        uint16_t nuh_layer_id        : 6;
        uint16_t nal_unit_type       : 6;
        uint16_t forbidden_zero_bit  : 1;
    };

    struct Nalu
    {
        NaluHeader nalu_header;
        char*      nalu_data;
        uint16_t   nalu_len; /*include the header len*/
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

        std::vector<std::string> NaluType = {
        "TRAIL_N",       
        "TRAIL_R",       
        "TSA_N",
        "TLA",
        "STSA_N",
        "STSA_R",
        "RADL_N",
        "DLP",
        "RASL_N",
        "TFD",
        "RESERVED_10",
        "RESERVED_11",
        "RESERVED_12",
        "RESERVED_13",
        "RESERVED_14",
        "RESERVED_15",
        "BLA",
        "BLANT",
        "BLA_N_LP",
        "IDR",
        "IDR_N_LP",
        "CRA",
        "RESERVED_22",
        "RESERVED_23",
        "RESERVED_24",
        "RESERVED_25",
        "RESERVED_26",
        "RESERVED_27",
        "RESERVED_28",
        "RESERVED_29",
        "RESERVED_30",
        "RESERVED_31",
        "VPS",
        "SPS",
        "PPS",
        "ACCESS_UNIT_DELIMITER",
        "EOS",
        "EOB",
        "FILLER_DATA",
        "SEI",
        "SEI_SUFFIX",
        "RESERVED_41",
        "RESERVED_42",
        "RESERVED_43",
        "RESERVED_44",
        "RESERVED_45",
        "RESERVED_46",
        "RESERVED_47",
        "UNSPECIFIED_48",
        "UNSPECIFIED_49",
        "UNSPECIFIED_50",
        "UNSPECIFIED_51",
        "UNSPECIFIED_52",
        "UNSPECIFIED_53",
        "UNSPECIFIED_54",
        "UNSPECIFIED_55",
        "UNSPECIFIED_56",
        "UNSPECIFIED_57",
        "UNSPECIFIED_58",
        "UNSPECIFIED_59",
        "UNSPECIFIED_60",
        "UNSPECIFIED_61",
        "UNSPECIFIED_62",
        "UNSPECIFIED_63",
        "INVALID"
    };
};

enum CODEC
{
    CODEC_H264,
    CODEC_H265,
    CODEC_AAC
};



}; /*end of namespace rtp*/

#endif /* NALU_H */
