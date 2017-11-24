#include <iostream>
#include <gtest/gtest.h>
#include "html_printer.h"
#include "html_element.h"
#include "html_node.h"
#include "html_vnode.h"

using namespace html;

void print(const Node& node)
{
    Printer printer(&std::cout);
    printer << node;
    std::cout<<std::endl;
}

TEST(html,construct)
{
    Node html("html");
    Node div("div","text");
    Node span("span",1);
    Node span1("span",1.1);
}

TEST(html,value)
{
    Node div("div","text");
    div.value("");
    div.value(1);
    div.value(1.1);
}

TEST(html, attri)
{
    Node div("div","text");
    div.attri("id",1);
    div.attri("back_ground","red");
    div.attri("width",3.4);
    div.attri("height","100%");
}

TEST(html, children)
{
    Node html("html");
    Node div("div");
    Node img("img");

    div.append_child(img);
    div.append_child(img);
    html.append_child(div);
    print(html);
}

TEST(html, children1)
{
    Node html("html");
    html.append_child("head");
    html.append_child("body");
    print(html);
}

TEST(html,attri_and_value)
{
    Node doc("html");
    doc > Node("div").attri("width",1).value("abcdef").attri("height","100%");
    print(doc);
}

TEST(html,element)
{
    Html doc;
    doc > (Head() > Title()) + (Body() > (Span() + Div()*5));
    print(doc);
}

TEST(html,node_linked)
{
    Node doc("html");
    doc > Body() > Div() > Span() > Img();
    print(doc);
    //print(Html() > Body() > Div() > Span() > Img()); /* wrong linded operat returned the last node*/
}

TEST(html,node_plus_node)
{
    print(Html() > Div() + Div());
}

TEST(html,node_mul)
{
    Node doc("html");
    doc > Node("div").attri("width",1).value("abcdef").attri("height","100%")*5;
    print(doc);
}

TEST(html,node_plus_vnode1)
{
    print(Html() > Div() + Span() + Img());
}

TEST(html,node_plus_vnode2)
{
    print(Html() > Div() + (Span() + Img()));
}

TEST(html,normal_html)
{
    auto doc = Html();
    doc > (Head() > Title()) + (Body() > Div() > (Span() + Img()));
    print(doc);
}

TEST(html,print)
{
    Node div("div","text");
    div.attri("id",1);
    div.attri("back_ground","red");
    div.attri("width",3.4);
    print(div);
}

