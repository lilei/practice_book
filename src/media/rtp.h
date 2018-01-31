#include <gtest/gtest.h>

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

struct RtpHeader
{
#ifdef BIG_ENDIAN
    uint8_t  version     : 2;
    uint8_t  padding     : 1;
    uint8_t  extension   : 1;
    uint8_t  csrccount   : 4;

    uint8_t  marker      : 1;
    uint8_t  payloadtype : 7;
#else // little endian
    uint8_t csrccount    : 4;
    uint8_t extension    : 1;
    uint8_t padding      : 1;
    uint8_t version      : 2;
    
    uint8_t payloadtype  : 7;
    uint8_t marker       : 1;
#endif // BIG_ENDIAN
    uint16_t sequencenumber;
    uint32_t timestamp;
    uint32_t ssrc;
};

class RtpPacket 
{
public:
    RtpPacket(char* data,int len)
        :data_(data),len_(len)
    {
    }

    ~RtpPacket()
    {
    }

    RtpHeader* header()
    {
        return reinterpret_cast<RtpHeader*> (data_);
    }

    void payload(char** payload,int& payload_len)
    {
        int header_len = sizeof(RtpHeader) + 4 * header()->csrccount;
        *payload = data_ + header_len;
        payload_len = len_ - header_len;
    }

private:
    char* data_;
    int len_;
};
