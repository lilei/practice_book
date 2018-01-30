#ifndef PARSER_ERROR_H
#define PARSER_ERROR_H

#include "list_token.h"
#include <string>
#include <sstream>


class ParserError
{
public:
    ParserError()
        :invalid_char_(0),char_error_(false) 
    {}

    ParserError& expect(int expect_type)
    {
        expect_ = Token::token_name(expect_type);
        return *this;
    }

    ParserError&  expect(std::string expect_str)
    {
        expect_ = expect_str;
        return *this;
    }

    ParserError& found(const Token& token)
    {
        found_ = token;
        return *this;
    }

    ParserError& found(char c)
    {
        char_error_ = true;
        invalid_char_ = c;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const ParserError& e);

private:
    std::string expect_;
    Token found_;

    bool char_error_;
    char invalid_char_;
};

#endif /* PARSER_ERROR_H */
