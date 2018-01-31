#ifndef LIST_LEXER_H
#define LIST_LEXER_H

#include "parser_lexer.h"
#include "parser_error.h"
#include "list_token.h"
#include <string>
#include <vector>


/*
    NAME     : LETTER+ ;                 // name is sequence of >=1 letter
    LETTER   : 'a'..'z'|'A'..'Z';        // define what a letter is
    WS       : (' '|'\t'|'\n'|'\r')+ {skip();} ; // throw out whitespace
*/

class ListLexer :public Lexer
{
public:
    explicit ListLexer(std::string input)
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
            case '=':
                consume();
                return Token(EQUAL,"=");
            default:
                if (is_letter())
                {
                    return name();
                }
                else
                {
                    throw ParserError().found(c);
                }
            }
        }
        return Token(EOF_TYPE, "<EOF>");
    }

private:
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

#endif /* LIST_LEXER_H */
