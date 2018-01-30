#ifndef LIST_PARSER_H
#define LIST_PARSER_H

#include "list_token.h"
#include "list_lexer.h"
#include <string>


class ListParser
{
public:
    ListParser(std::string input);

    /*
        list     : '[' elements ']' ;        // match bracketed list
        elements : element (',' element)* ;  // match comma-separated list
        element  : NAME | list ;             // element is name or nested list
    */
    void list();
    void elements();
    void element();

private:
    void match(int x);
    void consume();

    ListLexer lexer_;
    Token lookahead_;
};
#endif /* LIST_PARSER_H */
