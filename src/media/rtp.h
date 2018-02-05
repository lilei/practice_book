#ifndef RTP_H
#define RTP_H

#include "bitstream.h"

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

class RtpHeader 
{
public:
    RtpHeader(BitStream* input)
        :input_(input)
    {
    }

    void parse()
    {
        uint8_t version         = input_->read_field<uint8_t>(2);
        uint8_t padding         = input_->read_field<uint8_t>(1);
        uint8_t extension       = input_->read_field<uint8_t>(1);
        uint8_t csrccount       = input_->read_field<uint8_t>(4);

        uint8_t marker          = input_->read_field<uint8_t>(1);
        uint8_t payloadtype     = input_->read_field<uint8_t>(7);

        uint16_t sequencenumber = input_->read_field<uint16_t>(16);
        uint32_t timestamp      = input_->read_field<uint32_t>(32);
        uint32_t ssrc           = input_->read_field<uint32_t>(32);
    }

private:
    BitStream* input_;
};

#endif /* RTP_H */
