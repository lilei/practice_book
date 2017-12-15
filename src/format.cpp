#include <gtest/gtest.h>
#include <sstream>

class stringparser
{
public:
    explicit stringparser(std::string str)
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
            char temp;
            if (!(s_ >> temp))
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
    std::string url = "rtsp://127.0.0.1:8000/file/sessions:[1]:[2]";

    std::string proto;
    std::string ip;
    int port = 0;
    int channel = 0;
    int sub_channel = 0;

    try
    {
        stringparser(url)
            .read_until(proto, ':')
            .consume("//")
            .read_until(ip, ':')
            .read_int(port)
            .consume("/file")
            .consume("/sessions")
            .consume(':')
            .consume('[') .read_int(channel) .consume(']')
            .consume(':')
            .consume('[') .read_int(sub_channel) .consume(']')
            .end();
    }
    catch (const stringparser::exception&)
    {
        EXPECT_TRUE(false);
    }

    EXPECT_EQ("rtsp",proto);
    EXPECT_EQ("127.0.0.1",ip);
    EXPECT_EQ(8000,port);
    EXPECT_EQ(1,channel);
    EXPECT_EQ(2,sub_channel);
}
