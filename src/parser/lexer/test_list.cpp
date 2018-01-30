#include "list_lexer.h"
#include "list_parser.h"
#include <gtest/gtest.h>

TEST(list_parser,lexer)
{
    ListLexer lexer("[a, b,lilei,   haha]");

    try
    {
        Token t = lexer.next_token();
        while (t.type() != EOF_TYPE)
        {
            std::cout << t;
            t = lexer.next_token();
        }
        std::cout << t;
    }
    catch (const ParserError& e)
    {
        std::cout << e;
    }
}

TEST(list_parser,lexer_error)
{
    ListLexer lexer("[li &lei]");
    try
    {
        Token t = lexer.next_token();
        while (t.type() != EOF_TYPE)
        {
            std::cout << t;
            t = lexer.next_token();
        }
        std::cout << t;
    }
    catch (const ParserError& e)
    {
        std::cout << e;
    }
}

TEST(list_parser, parser)
{
    ListParser parser("[a,b]");
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}

TEST(list_parser, parser_error)
{
    ListParser parser("[a,,b]");
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}

