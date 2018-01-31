#ifndef LIST_PARSER_EX_H
#define LIST_PARSER_EX_H

#include "list_token.h"
#include "list_lexer.h"
#include <string>
#include <vector>
#include <assert.h>

class LookAheadArray
{
public:
    LookAheadArray(int len)
        :size_(len),p_(0)
    {}
    
    const Token& operator[](int i) const
    {
        assert(i <= size_);
        return array_[(p_ + i - 1) % size_];
    }

    void write(Token& t)
    {
        if (array_.size() == p_)
        {
            array_.push_back(t);
        }
        else
        {
            array_[p_] = t;
        }
        p_ = (p_ + 1) % size_;
    }
private:
    std::vector<Token> array_;
    int size_;
    int p_;
};

class ListParserEx
{
public:
    ListParserEx(std::string input,int k);

    /*
        list     : '[' elements ']' ;        // match bracketed list
        elements : element (',' element)* ;  // match comma-separated list
        element  : NAME        | 
                   NAME = NAME |
                   list ;             // element is name or nested list
    */
    void list();
    void elements();
    void element();

private:
    void match(int x);
    void consume();

    ListLexer lexer_;
    LookAheadArray lookahead_;
};
#endif /* LIST_PARSER_EX_H */
