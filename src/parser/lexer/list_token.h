#ifndef LIST_TOKEN_H
#define LIST_TOKEN_H

#include <vector>
#include <string>
#include <ostream>


const static int EOF_TYPE  = 1;
const static int NAME      = 2;
const static int COMMA     = 3;
const static int LBRACK    = 4;
const static int RBRACK    = 5;
const static int EQUAL     = 6;
const std::vector<std::string> token_name_ = { "n/a", "<EOF>", "NAME", "COMMA", "LBRACK", "RBRACK","EUQAL" };

class Token
{
public:
    Token() {}

    Token(int type,const char* text)
        :type_(type),text_(text)
    {}

    int type() const
    {
        return type_;
    }

    std::string text() const
    {
        return text_;
    }

    friend std::ostream& operator<<(std::ostream& out, Token token);

    static std::string token_name(int type)
    {
        return token_name_[type];
    }

protected:
    int type_;
    std::string text_;
};


#endif /* LIST_TOKEN_H */
