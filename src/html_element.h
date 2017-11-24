#ifndef HTML_ELEMENT_H
#define HTML_ELEMENT_H
#include "html_node.h"

namespace html
{ 

class document :public Node
{
public:
    document() :Node("html")
    {
        append_child("head").append_child("title");
        append_child("body");
    }
};

class div :public Node
{
public:
    div() :Node("div") { }
};

class img :public Node
{
public:
    img(const char* src) 
        :Node("img") 
    { 
        attri_[src] = src;
    }
};

class Td :public Node
{
public:
    Td(const char* text) 
        :Node("td",text) {}
};

class Tr :public Node
{
public:
    Tr() :Node("tr") {}
};

class Table :public Node
{
public:
    Table() :Node("table") {}
};

} // end namespace

#endif /* HTML_ELEMENT_H */
