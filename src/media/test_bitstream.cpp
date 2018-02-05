#include "media/bitstream.h"
#include "media/rtp_h264.h"
#include "media/rtp_h265.h"
#include "media/rtp.h"
#include "network/network.h"
#include <gtest/gtest.h>

class RtpStream :public BitStream
{
public:
    RtpStream()
        :BitStream(1500,2),client_(NULL,5555)
    {
        client_.listen();
    }
    ~RtpStream()
    {
    }

    int on_write(char* buff, int size)
    {
        return client_.recv(buff, size);
    }
private:
    UdpClient client_;
};


TEST(BitStream,rtp_h264)
{
    RtpStream input;

    rtp::h264::Packet packet(&input);
    try
    {
        while (true)
        {
            packet.parse();
        }
    }
    catch (BitStreamError)
    {
    }
}

TEST(BitStream,rtp_h265)
{
    RtpStream input;

    rtp::h265::Packet packet(&input);
    try
    {
        while (true)
        {
            packet.parse();
        }
    }
    catch (BitStreamError)
    {
    }
}
