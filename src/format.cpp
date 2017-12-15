#include <gtest/gtest.h>
#include <sstream>

bool read_until(std::istringstream &ss, std::string &str,const char c)
{
    char temp;
    for (;!ss.eof();)
    {
        ss >> temp;
        if (c == temp)
        {
            return true;
        }
        str += temp;
    }
    return false;
}

bool consume(std::istringstream &ss, const char c)
{
    char temp;
    ss >> temp;
    if (c == temp)
    {
        return true;
    }
    else
    {
        return false;
    }
}

TEST(format,urlpaser)
{
    std::string url = "rtsp://127.0.0.1:8000/file/sessions/ad34wdwdaw12jbj232jnijiijwad2323gh";
    std::istringstream ss (url);

    std::string proto;
    std::string ip;
    int port = 0;
    std::string rest;

    read_until(ss, proto, ':');
    consume(ss,'/');
    consume(ss,'/');
    read_until(ss, ip, ':');
    ss >> port;

    std::cout << "proto: " << proto << std::endl;
    std::cout << "ip: " << ip << std::endl;
    std::cout << "port: " << port << std::endl;
}