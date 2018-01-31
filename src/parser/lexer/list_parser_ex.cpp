#include "list_parser_ex.h"
#include "parser_error.h"

ListParserEx::ListParserEx(std::string input,int k)
    :lexer_(input),lookahead_(k)
{
    for (int i = 0;i < k;i++)
    {
        consume();
    }
}

void ListParserEx::list()
{
    match(LBRACK);
    elements();
    match(RBRACK);
}

void ListParserEx::elements()
{
    element();
    while (COMMA == lookahead_[1].type())
    {
        match(COMMA);
        element();
    }
}

void ListParserEx::element()
{
    if (lookahead_[1].type() ==  NAME && lookahead_[2].type() == EQUAL)
    {
        match(NAME);
        match(EQUAL);
        match(NAME);
    }
    else if (lookahead_[1].type() == NAME)
    {
        match(NAME);
    }
    else if (lookahead_[1].type() == LBRACK)
    {
        list();
    }
    else
    {
        throw ParserError().expect("name or list or name=name").found(lookahead_[1]);
    }
}

void ListParserEx::match(int x)
{
    if (lookahead_[1].type() == x)
    {
        consume();
    }
    else
    {
        throw ParserError().expect(x).found(lookahead_[1]);
    }
}

void ListParserEx::consume()
{
    lookahead_.write(lexer_.next_token());
}
