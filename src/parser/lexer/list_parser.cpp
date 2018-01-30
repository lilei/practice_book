#include "list_parser.h"
#include "parser_error.h"

ListParser::ListParser(std::string input)
    :lexer_(input),lookahead_(lexer_.next_token())
{
}

void ListParser::list()
{
    match(LBRACK);
    elements();
    match(RBRACK);
}

void ListParser::elements()
{
    element();
    while (COMMA == lookahead_.type())
    {
        match(COMMA);
        element();
    }
}

void ListParser::element()
{
    if (lookahead_.type() == NAME)
    {
        match(NAME);
    }
    else if (lookahead_.type() == LBRACK)
    {
        list();
    }
    else
    {
        throw ParserError().expect("name or list").found(lookahead_);
    }
}

void ListParser::match(int x)
{
    if (lookahead_.type() == x)
    {
        consume();
    }
    else
    {
        throw ParserError().expect(x).found(lookahead_);
    }
}

void ListParser::consume()
{
    lookahead_ = lexer_.next_token();
}
