#include <iostream>
#include <gtest/gtest.h>
#include "html_printer.h"
#include "html_element.h"
#include "html_node.h"

using namespace html;

void print(const Node& node)
{
    Printer printer(&std::cout);
    printer << node;
    std::cout<<std::endl;
}

TEST(html,node_construct)
{
    Node html("html");
    Node div("div","text");
    Node span("span",1);
    Node span1("span",1.1);
}

TEST(html,node_value)
{
    Node div("div","text");
    div.value("");
    div.value(1);
    div.value(1.1);
}

TEST(html, node_attri)
{
    Node div("div","text");
    div.attri("id",1);
    div.attri("back_ground","red");
    div.attri("width",3.4);
    div.attri("height","100%");
}

TEST(html, node_children)
{
    Node html("html");
    Node div("div");
    Node img("img");

    div.append_child(img);
    div.append_child(img);
    html.append_child(div);
    print(html);
}

TEST(html, node_children1)
{
    Node html("html");
    html.append_child("head");
    html.append_child("body");
    print(html);
}

TEST(html, node_linked)
{
    Node html("html");
    html.append_child("body").append_child("div").append_child("span").append_child("img");
    print(html);
}

TEST(html, node_operator)
{
    Node html("html");
    html>"body">"div">"span">"img";
    print(html);
}

TEST(html, node_operator1)
{
    auto doc = Node("html");
    auto head = Node("head");
    auto title = Node("title");
    auto body = Node("body");
    auto div = Node("div");
    auto span = Node("span");
    auto img = Node("img");
    doc > (head > title) > (body > (div > (span > img)));
    print(doc);
}

TEST(html, document)
{
    document  doc;
    print(doc);
}

TEST(html, table)
{
    Table  table;
    for (int i = 0;i < 5;i++)
    {
        Tr tr;
        for (int j = 0;j < 3;j++)
        {
            Td td(std::to_string(i * 3 + j).c_str());
            tr > td;
        }
        table > tr;
    }
    print(table);
}

TEST(html,link)
{
    Node doc("html");
    doc > Node("body") > Node("div") > Node("span") > Node("img");
    print(doc);
}

TEST(html,vnode)
{
    Node doc("html");
    doc > (Node("head") > Node("title")) + 
          (Node("body") > 
                         Node("div") +
                         Node("div"));
    print(doc);
}

TEST(html,attri_and_value)
{
    Node doc("html");
    doc > Node("div").attri("width",1).value("abcdef").attri("height","100%");
    print(doc);
}

TEST(html,multi)
{
    Node doc("html");
    doc > Node("div").attri("width",1).value("abcdef").attri("height","100%")*5;
    print(doc);
}

TEST(html,node_print)
{
    Node div("div","text");
    div.attri("id",1);
    div.attri("back_ground","red");
    div.attri("width",3.4);
    print(div);
}

