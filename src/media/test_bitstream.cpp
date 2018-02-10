#include "media/bitstream.h"
#include "media/rtp_h264.h"
#include "media/rtp_h265.h"
#include "media/rtp.h"
#include "media/ps_parser.h"
#include "network/network.h"
#include "rtp_code.h"
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
        :BitStream(1500, 4),fs_(file_name,std::fstream::binary)
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
    //PsStream input("../resource/sintel.ps");
    PsStream input("../resource/new_h264.mp4");
    //PsStream input("../resource/264_test.mp4");
    //PsStream input("../resource/data_ps_1.mp4");
    //PsStream input("../resource/265.mp4");
    PSParser ps;
    RtpEncoder rtp;
    std::ofstream rtp_file("output_rtp.mp4",std::ios::binary);
    rtp.on_packet_ready = [&](char* data,int len)
    {
        rtp_file.write(data,len);
        //std::cout << "rtp len: " << len << std::endl;
    };

    uint16_t seq = 0;
    uint32_t timestamp = 0;
    bool end_of_es = false;

    ps.on_timestamp = [&](uint32_t timestamp) 
    {
        rtp.timestamp(timestamp);
    };

    ps.on_es_begin = [&]()
    {
        end_of_es = false;
    };

    ps.on_es_end = [&]()
    {
        end_of_es = true;
    };


    //std::ofstream file("../resource/output.h265",std::ios::binary);
    ps.on_video_es = [&](char* data,int len)
    {
        //start of a nal unit
        if (data[0] == 0 && data[1] == 0 && data[2] == 0 &&data[3] == 1)
        {
            data += 4;
            len -= 4;
            rtp.nalu_begin(true);
        }
        else
        {
            rtp.nalu_begin(false);
        }
        rtp.push_data(data,len);
    };

    ps.on_stream_type = [](uint8_t stream_id,uint8_t stream_type)
    {
        if (stream_id >= 0xE0 && stream_id <= 0xEF)
        {
            std::cout << "video codec: ";
            switch (stream_type)
            {
            case 0x1B:
                std::cout << "H.264" << std::endl;
                break;
            case 0x24:
                std::cout << "H.265" << std::endl;
                break;
            default:
                std::cout << stream_type << std::endl;
                break;
            }
        } 
        else if (stream_id >= 0xC0 && stream_id <= 0xCF)
        {
            std::cout << "audio codec: " << (int)stream_type << std::endl;
        }
    };

    try
    {
        ps.parse(input);
    }
    catch (BitStreamError& )
    {
    }
}
