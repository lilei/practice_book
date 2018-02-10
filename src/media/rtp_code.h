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

        write_ = data_ + sizeof(RtpHeader);
        nalu_begin_ = true;
        fu_ = false;
    }

    ~RtpEncoder()
    {}

    std::function<void(char*, int)> on_packet_ready;


    void nalu_begin(bool nalu_begin)
    {
        nalu_begin_ = nalu_begin;
    }

    void nalu_end()
    {
    }

    void timestamp(uint32_t timestamp)
    {
        rtp_header()->timestamp = timestamp;
    }

    void push_data(char* data,int len)
    {
        /*the previous rtp packet is ready*/ 
        if (nalu_begin_ && !empty())
        {
            rtp_header()->marker = 1;
            if (fu_)
            {
                fu_header()->fu_e = 1;
                fu_ = false;
            }
            packet_ready();
            rtp_header()->marker = 0;
        }
        if (len <= free_size())
        {
            if (fu_)
            {
                fu_header()->fu_s = 0;
                fu_header()->fu_e = 0;
                fu_header()->nalu_type = 28;
            }
            memcpy(write_, data, len);
            write_ += len;
            return;
        }
        else
        {
            if (!fu_)
            {
                fu_ = true;
                fu_header()->fu_s = 1;
                fu_header()->nalu_type = 28;
            }
            if (0 == free_size())
            {
                packet_ready();
            }
            write_ += 1;
            int write_len = free_size();
            memcpy(write_,data,write_len);
            packet_ready();
            push_data(data_ + write_len,len - write_len);
        }
    }

private:
    inline RtpHeader* rtp_header()
    {
        return (RtpHeader*)data_;
    }

    char* rtp_payload()
    {
        return data_ + sizeof(RtpHeader);
    }
    
    int free_size()
    {
        return data_ + PACKET_LEN - write_;
    }

    bool empty()
    {
        if (write_ - data_ == sizeof(RtpHeader))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void packet_ready()
    {
        on_packet_ready(data_,write_ - data_);
        if (fu_)
        {
            write_ = rtp_payload() + 1;
        }
        else
        {
            write_ = rtp_payload();
        }
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

    void fragment(char* data,int len)
    {
        packet_ready();
    }

    char data_[PACKET_LEN];
    char* write_; /*current write pos*/
    bool nalu_begin_;
    bool fu_;

};
