#include <gtest/gtest.h>
#include "media/rtp.h"
#include "media/bit_utils.h"
#include "network/network.h"
#include "media/rtp_h264.h"
#include "media/rtp_h265.h"
#include "media/bitstream.h"
#include <fstream>

#pragma comment(lib, "winmm.lib")

TEST(rtp,rtp)
{
    UdpClient client(NULL,5555);
    client.listen();
    char buff[1500] = { 0 };
    int len = 1500;
    while (true)
    {
        int recv_len = client.recv(buff, len);
        RtpPacket packet(buff,recv_len);
        RtpHeader* header = packet.header();
        char* payload = NULL;
        int payload_len = 0;
        packet.payload(&payload,payload_len);
        //rtp::h264::RtpPayload(payload,payload_len);
        rtp::h265::RtpPayload(payload, payload_len);
    }
}

TEST(rtp,aac)
{
    std::ofstream fs("out.aac",std::ios_base::binary);

    UdpClient client(NULL,8888);
    client.listen();
    char buff[1500] = { 0 };
    int len = 1500;
    while (true)
    {
        int recv_len = client.recv(buff, len);
        static DWORD tick = 0;
        int now = timeGetTime();
        std::cout << now - tick << std::endl;
        tick = now;

        std::cout << "packet size: " << recv_len << std::endl;
        if (recv_len < 12)
        {
            break;
        }
            
        uint16_t au_header_len = byte_to_number<uint16_t>(buff + 12,2); 
        std::cout << "au header len: " << static_cast<int>(au_header_len) << std::endl;
        fs.write(buff + 12, recv_len - 12);
        RtpPacket packet(buff,recv_len);
        RtpHeader* header = packet.header();
        uint8_t version = packet.header()->version;
        //std::cout << "ssrc: " << packet.header()->ssrc;
        //std::cout << "      cc: " << static_cast<int>(packet.header()->csrccount);
        std::cout << "      timestamp: " << packet.header()->timestamp << std::endl;
        std::cout << "      pt: "  << (int)packet.header()->payloadtype << std::endl;
        //std::cout << *reinterpret_cast<int*>(buff + 12) <<std::endl;
        switch (packet.header()->payloadtype)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18: std::cout << "Audio" << std::endl; break;
            case 31: std::cout << "H.261" << std::endl; break;
            case 32: std::cout << "MPV"   << std::endl; break;
            case 33: std::cout << "MP2T"  << std::endl; break;
            case 34: std::cout << "H.263" << std::endl; break;
            case 96: std::cout << "H.264" << std::endl; break;
            default: std::cout << "other" << std::endl; break;
        }
    }
}

