#include <gtest/gtest.h>
#include <sstream>

class stringparser
{
public:
    stringparser(std::string str)
        :s_(str) {}

    stringparser& read_until(std::string& got,const char until)
    {
        char temp;
        for (;!s_.eof();)
        {
            s_ >> temp;
            if (until == temp)
            {
                return *this;
            }
            got += temp;
        }
        bad();
        return *this;
    }

    stringparser& read_int(int& got)
    {
        s_ >> got;
        if (s_.eof())
        {
            bad();
        }
        return *this;
    }


    stringparser& consume(char c)
    {
        char temp;
        s_ >> temp;
        if (c != temp)
        {
            bad();
        }
        return *this;
    }

    stringparser& consume(const std::string& str)
    {
        for (size_t i = 0;i < str.size();i++)
        {
            char temp = 0;
            if (!s_ >> temp)
            {
                bad();
            }
            if (str.at(i) != temp)
            {
                bad();
            }
        }
        return *this;
    }

    void end()
    {
        char temp;
        s_ >> temp;
        if (!s_.eof())
        {
            bad();
        }
    }

    class exception
    {
    };
private:
    void bad()
    {
        throw exception();
    }

    std::istringstream s_;
};


TEST(format,urlpaser)
{
    std::string url = "rtsp://127.0.0.1:8000/file/sessions/ad34wdwdaw12jbj232jnijiijwad2323gh";
    stringparser parser(url);

    std::string proto;
    std::string ip;
    int port = 0;
    std::string rest;

    try
    {
        parser.read_until(proto, ':')
            .consume('/')
            .consume('/') 
            .read_until(ip,':')
            .read_int(port)
            .consume("/file")
            .consume("/sessions/");
    }
    catch (const stringparser::exception&)
    {
    }


    std::cout << "proto: " << proto << std::endl;
    std::cout << "ip: " << ip << std::endl;
    std::cout << "port: " << port << std::endl;
}
