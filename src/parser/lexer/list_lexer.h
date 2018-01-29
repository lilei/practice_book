#include "parser_lexer.h"
#include "parser_token.h"
#include "parser_error.h"
#include <string>
#include <vector>

using namespace parser;

class ListLexer :public Lexer
{
public:
    const int EOF_TYPE  = 1;
    const int NAME      = 2;
    const int COMMA     = 3;
    const int LBRACK    = 4;
    const int RBRACK    = 5;
    const std::vector<std::string> token_name_ = { "n/a", "<EOF>", "NAME", "COMMA", "LBRACK", "RBRACK" };

    ListLexer(std::string input)
        :Lexer(input)
    {}


    Token next_token() 
    {
        while (c != eof) 
        {
            switch (c) 
            {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                white_space();
                continue;
            case ',':
                consume();
                return Token(COMMA, ",");
            case '[':
                consume();
                return Token(LBRACK, "[");
            case ']':
                consume();
                return Token(RBRACK, "]");
            default:
                if (is_letter())
                {
                    return name();
                }
                else
                {
                    throw Error(Error::INVALID_CHAR,c);
                }
            }
        }
        return Token(EOF_TYPE, "<EOF>");
    }

private:
    /** NAME : ('a'..'z'|'A'..'Z')+; // NAME is sequence of >=1 letter */
    Token name() 
    {
        std::string buf;
        do 
        {
            buf.push_back(c);
            consume(); 
        } while (is_letter());
        return Token(NAME, buf.c_str());
    }

    void white_space()
    {
        while (' ' == c || '\t' == c || '\r' == c || '\n' == c)
        {
            consume();
        }
    }

};
