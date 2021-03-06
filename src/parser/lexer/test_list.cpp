#include "list_lexer.h"
#include "list_parser.h"
#include "list_parser_ex.h"
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
TEST(list_parser, parser_nest_list)
{
    ListParser parser("[a,[c,d],b]");
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

TEST(list_parser, parser_error1)
{ 
    ListParser parser("[a,[b]");
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}


TEST(list_parser, parser_error2)
{ 
    ListParser parser("[a,&b]");
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}

TEST(list_parserex,parser)
{
    ListParserEx parser("[a,c = d,b]",2);
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}

TEST(list_parserex,parser_nest)
{
    ListParserEx parser("[a,c = d,[d,f=g],b]",2);
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}


TEST(list_parserex,parser_error1)
{
    ListParserEx parser("[a,c = 1,b]",2);
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}

TEST(list_parserex,parser_error2)
{
    ListParserEx parser("[a,c = d e,b]",2);
    try
    {
        parser.list();
    }
    catch (const ParserError&e)
    {
        std::cout << e;
    }
}
