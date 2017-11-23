#include "html_printer.h"
#include <iostream>
#include <gtest/gtest.h>

using namespace html;

TEST(html_printer,node_construct)
{
    Node html("html");
    Node div("div","text");
    Node span("span",1);
    Node span1("span",1.1);
}

TEST(html_printer,node_value)
{
    Node div("div","text");
    div.value("");
    div.value(1);
    div.value(1.1);
}

TEST(html_printer, node_attri)
{
    Node div("div","text");
    div.attri("id",1);
    div.attri("back_ground","red");
    div.attri("width",3.4);
    div.attri("height","100%");
}

TEST(html_printer, node_children)
{
    Node html("html");
    Node div("div");
    Node img("img");

    div.append_child(img);
    div.append_child(img);
    html.append_child(div);
    Printer printer(&std::cout);
    printer << html;
    std::cout << std::endl;
}

TEST(html_printer, node_children1)
{
    Node html("html");
    html.append_child("head");
    html.append_child("body");
    Printer printer(&std::cout);
    printer << html;
    std::cout << std::endl;
}

TEST(html_printer, node_linked)
{
    Node html("html");
    html.append_child("body").append_child("div").append_child("span").append_child("img");
    Printer printer(&std::cout);
    printer << html;
    std::cout << std::endl;
}

TEST(html_printer, node_operator)
{
    Node html("html");
    html>>"body">>"div">>"span">>"img";
    Printer printer(&std::cout);
    printer << html;
    std::cout << std::endl;
}

TEST(html_printer, node_operator1)
{
    auto doc = Node("html");
    auto head = Node("head");
    auto title = Node("title");
    auto body = Node("body");
    auto div = Node("div");
    auto span = Node("span");
    auto img = Node("img");
    doc >> (head >> title) >> (body >> (div >> (span >> img)));
    Printer printer(&std::cout);
    printer << doc;
    std::cout << std::endl;
}

TEST(html_printer, document)
{
    document  doc;
    Printer printer(&std::cout);
    printer << doc;
    std::cout << std::endl;
}

TEST(html_printer, table)
{
    Table  table;
    for (int i = 0;i < 5;i++)
    {
        Tr tr;
        for (int j = 0;j < 3;j++)
        {
            Td td(std::to_string(i * 3 + j).c_str());
            tr >> td;
        }
        table >> tr;
    }
    Printer printer(&std::cout);
    printer << table;
    std::cout << std::endl;
}

TEST(html_printer,node_print)
{
    Node div("div","text");
    div.attri("id",1);
    div.attri("back_ground","red");
    div.attri("width",3.4);
    Printer printer(&std::cout);
    printer << div;
    std::cout << std::endl;
}

