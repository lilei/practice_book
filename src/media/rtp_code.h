#ifndef RTP_CODE_H
#define RTP_CODE_H

#include <functional>
#include <fstream>

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

    struct NaluHeader
    {
        uint8_t nalu_type : 5;
        uint8_t nalu_nri : 2;
        uint8_t nalu_forbiddern : 1;
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
        header->ssrc = 0x44332211;

        buff_ = new char[1024 * 1024];
        write_ = buff_;
        seq_ = 0;
        test_file_.open("test.h264",std::ios::binary);
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

    void write_test()
    {
        test_file_.put(0);
        test_file_.put(0);
        test_file_.put(0);
        test_file_.put(1);

        int nalu_len = write_ - buff_;
        test_file_.write(buff_, nalu_len);
    }
    void nalu_end()
    {
        write_test();

        int nalu_len = write_ - buff_;
        if (nalu_len < PACKET_LEN - sizeof(RtpHeader))
        {
            single_nal();
        }
        else
        {
            fragment_nal();
        }
        write_ = buff_;
    }
    void timestamp(uint32_t timestamp)
    {
        rtp_header()->timestamp = htonl(timestamp);
    }
    void push_data(char* data,int len)
    {
        memcpy(write_, data, len);
        write_ += len;
    }

private:
    void single_nal()
    {
        rtp_header()->marker = 0;
        int nalu_len = write_ - buff_;
        memcpy(rtp_packet_ + sizeof(RtpHeader), buff_,nalu_len);
        packet_ready(sizeof(RtpHeader) + nalu_len);
    }

    void fragment_nal()
    {
        *nalu_header() = *(NaluHeader*)buff_;
        fu_header()->nalu_type = nalu_header()->nalu_type;
        nalu_header()->nalu_type = 28;

        char* p = buff_ + 1;
        int nalu_len = write_ - p;
        int fragment_len = PACKET_LEN - sizeof(RtpHeader) - 2;

        //the first fragment
        rtp_header()->marker = 0;
        fu_header()->fu_r = 0;
        fu_header()->fu_s = 1;
        fu_header()->fu_e = 0;
        memcpy(rtp_packet_ + sizeof(RtpHeader) + 2, p, fragment_len);
        packet_ready(sizeof(RtpHeader) + 2 + fragment_len);
        p += fragment_len;

        //the middle fragment
        while (p + fragment_len <= write_)
        {
            rtp_header()->marker = 0;
            fu_header()->fu_r = 0;
            fu_header()->fu_s = 0;
            fu_header()->fu_e = 0;
            memcpy(rtp_packet_ + sizeof(RtpHeader) + 2, p, fragment_len);
            packet_ready(sizeof(RtpHeader) + 2 + fragment_len);
            p += fragment_len;
        }

        //the last fragment
        rtp_header()->marker = 1;
        fu_header()->fu_r = 0;
        fu_header()->fu_s = 0;
        fu_header()->fu_e = 1;
        int last_len = write_ - p;
        memcpy(rtp_packet_ + sizeof(RtpHeader) + 2, p, last_len);
        packet_ready(sizeof(RtpHeader) + 2 + last_len);
        
    }

    void packet_ready(int len)
    {
        rtp_header()->sequencenumber = htons(seq_++);
        on_packet_ready(rtp_packet_,len);
    }

    inline RtpHeader* rtp_header()
    {
        return (RtpHeader*)rtp_packet_;
    }

    inline NaluHeader* nalu_header()
    {
        return (NaluHeader*)(rtp_packet_ + sizeof(RtpHeader));
    }

    inline fuHeader* fu_header()
    {
        return (fuHeader*)(rtp_packet_ + sizeof(RtpHeader) + 1);
    }

    char* buff_;
    char* write_; /*current write pos*/
    char rtp_packet_[PACKET_LEN];
    uint16_t seq_;
    std::ofstream test_file_;
};
