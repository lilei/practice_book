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
        got.clear();
        bad();
        return *this;
    }

    stringparser& read_int(int& got)
    {
        if (!(s_ >> got))
        {
            bad();
        }
        return *this;
    }

    stringparser& read_len(std::string& got, const int len)
    {
        char *temp = new char[len];
        if (!(s_.read(temp,len)))
        {
            bad();
        }
        got = temp;
        return *this;
    }

    stringparser& read_to_pos(std::string& got, const int pos)
    {
        int len = pos - s_.tellg();
        read_len(got,len);
    }

    stringparser& consume(const char c)
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

    bool test(const char c)
    {
        try
        {
            consume(c);
        }
        catch (const exception&)
        {
            s_.seekg(-1,std::ios_base::cur);
            return false;
        }
        return true;
    }

    bool split(const char c,std::string& left,std::string& right)
    {
        try
        {
            read_until(left,c);
        }
        catch (const exception&)
        {
            return false;
        }
        s_ >> right;
        return true;
    }

    bool split(const char c,std::vector<std::string>& strings)
    {
        std::string left;
        std::string right;
        if (!split(c, left, right))
        {
            return false;
        }
        strings.push_back(left);

        while (split(c, left, right))
        {
            strings.push_back(left);
        }

        std::string rest;
        s_ >> rest;

        return true;
    }

    std::string rest()
    {
        std::string rest;
        s_ >> rest;
        return rest;
    }

    stringparser& begin()
    {
        s_.seekg(0);
        return *this;
    }

    bool end()
    {
        char temp;
        s_ >> temp;
        if (s_.eof())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t pos()
    {
        return (size_t)s_.tellg();
    }

    void goto_pos(size_t pos)
    {
        s_.seekg(pos);
    }

    void go_back(size_t n)
    {
        s_.seekg((int)(0-n),std::ios_base::cur);
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

class UrlParser  
{
public:
    UrlParser(std::string& url)
        :url_(url),host_port_(-1)
    {}

    bool parse()
    {
        stringparser parser(url_);

        return parse_proto(parser) &&
            parse_host(parser)     &&
            parse_path(parser)     &&
            parse_query(parser);
    }
private:
    bool parse_proto(stringparser&  parser)
    {
        try
        {
            parser.read_until(proto_, ':')
                .consume("//");
        }
        catch (const stringparser::exception&)
        {
            return false;
        }
        return true;
    }

    bool parse_host(stringparser&  parser)
    {
        size_t pos = parser.pos();
        try
        {
            parser.read_until(host_addr_, ':')
                .read_int(host_port_);
            return true;
        }
        catch (const stringparser::exception&)
        {
            parser.goto_pos(pos);
        }

        try
        {
            parser.read_until(host_addr_, '/');
            parser.go_back(1);
            return true;
        }
        catch (const stringparser::exception&)
        {
            parser.goto_pos(pos);
        }

        try
        {
            parser.read_until(host_addr_, '?');
            return true;
        }
        catch (const stringparser::exception&)
        {
            parser.goto_pos(pos);
        }

        size_t pos2 = parser.pos();
        parser.goto_pos(pos);
        parser.read_to_pos(host_,pos2);
        return true;
    }

    bool parse_path(stringparser&  parser)
    {
        if (parser.end())
        {
            return true;
        }
        if (!parser.test('/'))
        {
            return true;
        }

        size_t pos = parser.pos();
        try
        {
            parser.read_until(path_,'?');
            parser.go_back(1);
        }
        catch (const stringparser::exception&)
        {
            parser.goto_pos(pos);
        }
        return true;
    }

    bool parse_query(stringparser&  parser)
    {
        if (parser.end())
        {
            return true;
        }
        if (!parser.test('?'))
        {
            return true;
        }

        size_t pos = parser.pos();
        std::string key;
        std::string value;
        while (!parser.end())
        {
            try
            {
                parser.read_until(key, '=');
            }
            catch (const stringparser::exception&)
            {
                return false;
            }

            try
            {
                parser.read_until(value, '&');
                continue;
            }
            catch (const stringparser&)
            {
            }
            value = parser.rest();
        }
        return true;
    }

    std::string& url_;
    std::string proto_;
    std::string  host_;
    std::string host_addr_;
    int host_port_;
    std::string path_;
    std::map<std::string, std::string> query_;
};

TEST(format,stringpaser)
{
    std::string url = "rtsp://127.0.0.1:8000/file/sessions:[1]:[2]";

    std::string proto;
    std::string ip;
    int port = 0;
    int channel = 0;
    int sub_channel = 0;

    try
    {
        EXPECT_TRUE(
            stringparser(url)
            .begin()
            .read_until(proto, ':')
            .consume("//")
            .read_until(ip, ':')
            .read_int(port)
            .consume("/file")
            .consume("/sessions")
            .consume(':')
            .consume('[').read_int(channel).consume(']')
            .consume(':')
            .consume('[').read_int(sub_channel).consume(']')
            .end());
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

TEST(format, urlpaser)
{
    //UrlParser parser("https://127.0.0.1:8000/test/1?a=1&b=2&c=ooxx");
    //parser.parse();
}
