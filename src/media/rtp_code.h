#ifndef RTP_CODE_H
#define RTP_CODE_H

#include <functional>

#endif /* RTP_CODE_H */
/*
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |V=2|P|X|  CC   |M|     PT      |       sequence number         |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                           timestamp                           |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |           synchronization source (SSRC) identifier            |
 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 |            contributing source (CSRC) identifiers             |
 |                             ....                              |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

class RtpEncoder
{
public:
    struct RtpHeader
    {
        uint8_t csrccount : 4;
        uint8_t extension : 1;
        uint8_t padding : 1;
        uint8_t version : 2;

        uint8_t payloadtype : 7;
        uint8_t marker : 1;

        uint16_t sequencenumber;
        uint32_t timestamp;
        uint32_t ssrc;
    };

    struct fuHeader
    {
        uint8_t nalu_type : 5;
        uint8_t fu_r      : 1;
        uint8_t fu_e      : 1;
        uint8_t fu_s      : 1;
    };


    static const int PACKET_LEN = 1400;

    RtpEncoder()
    {
        auto header = rtp_header();
        header->version = 2;
        header->padding = 0;
        header->extension = 0;
        header->csrccount = 0;
        
        header->marker = 0;
        header->payloadtype = 96;

        header->sequencenumber = 0;
        header->timestamp = 0;
        header->ssrc = 0;

        buff_ = new char[1024 * 1024];
        write_ = buff_;
    }

    ~RtpEncoder()
    {
        delete[]buff_;
    }

    std::function<void(char*, int)> on_packet_ready;


    void nalu_begin()
    {
        if (write_ != buff_)
        {
            nalu_end();
        }
    }
    void nalu_end()
    {
        int nalu_len = write_ - buff_;
        if (nalu_len < PACKET_LEN - sizeof(RtpHeader))
        {
            memcpy(rtp_packet_ + sizeof(RtpHeader), buff_, nalu_len);
            packet_ready(sizeof(RtpHeader) + nalu_len);
        }
        else
        {
            int unit_len = PACKET_LEN - sizeof(RtpHeader) - 1;
            int uint_size = nalu_len / unit_len;
            if (nalu_len % unit_len != 0)
            {
                uint_size += 1;
                unit_len = nalu_len / uint_size;
            }
        }
    }

    void timestamp(uint32_t timestamp)
    {
        rtp_header()->timestamp = timestamp;
    }

    void push_data(char* data,int len)
    {
    }

private:
    inline RtpHeader* rtp_header()
    {
        return (RtpHeader*)rtp_packet_;
    }


    void packet_ready(int len)
    {
        on_packet_ready(rtp_packet_,len);
        rtp_header()->sequencenumber++;
    }

    fuHeader* fu_header()
    {
        return (fuHeader*)rtp_payload();
    }

    void nalu(char* data,int len)
    {
        packet_ready();
    }

    int copy_data(char* data,int len)
    {
    }

    char* buff_;
    char* write_; /*current write pos*/
    char rtp_packet_[PACKET_LEN];
};
