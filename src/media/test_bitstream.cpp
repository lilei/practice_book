#include "media/bitstream.h"
#include "media/rtp_h264.h"
#include "media/rtp_h265.h"
#include "media/rtp.h"
#include "media/ps_parser.h"
#include "network/network.h"
#include <fstream>
#include <gtest/gtest.h>


class RtpStream :public BitStream
{
public:
    RtpStream()
        :BitStream(1500,2),client_(NULL,5555)
    {
        client_.listen();
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


class PsStream :public BitStream
{
public:
    PsStream(const char* file_name)
        :BitStream(1500, 2),fs_(file_name)
    {}

    int on_write(char* buff,int size)
    {
        if (fs_.eof())
        {
            return -1;
        }
        fs_.read(buff, size);
        return (int)fs_.gcount();
    }

private:
    std::ifstream fs_;
};


TEST(BitStream,ps)
{
    PsStream input("../resource/sintel.ps");

    PSParser ps(&input);
    try
    {
        ps.parse();
    }
    catch (BitStreamError)
    {
    }
}
