#include <string>

namespace parser
{

class Lexer
{
protected:
    Lexer(std::string& input)
        :input_(input), p(0), c(input[0])
    {}

    const static char eof = (char)-1;
    const static int eof_type = 1;

    void consume()
    {
        ++p;
        if (p >= input_.size())
        {
            c = eof;
        }
        else
        {
            c = input_[p];
        }
    }

    bool match(char x)
    {
        if (c == x)
        {
            consume();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool is_letter()
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A'&&c <= 'Z');
    }

protected:
    std::string input_;
    int p;
    char c;
};

}

