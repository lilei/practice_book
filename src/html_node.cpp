#include "html_node.h"
#include "html_printer.h"

namespace html
{ 
Node::Node(const char* name) 
    :name_(name),parent_(nullptr)
{}

Node::Node(const char* name,const char* value) 
    :name_(name),value_(value)
{}

Node::Node(const char* name,int value) 
    :name_(name),value_(std::to_string(value)),parent_(nullptr)
{}

Node::Node(const char* name,double value) 
    :name_(name),value_(std::to_string(value)),parent_(nullptr)
{}

Node::~Node()
{}

Node& Node::attri(const char* name, const char* value)
{
    attri_[name] = value;
    return *this;
}

Node& Node::attri(const char* name, int value)
{
    attri_[name] = std::to_string(value);
    return *this;
}

Node& Node::attri(const char* name, double value)
{
    attri_[name] = std::to_string(value);
    return *this;
}

Node& Node::value (const char* value)
{
    value_ = value;
    return *this;
}

Node& Node::value (int value)
{
    value_ = std::to_string(value);
    return *this;
}

Node& Node::value (double value)
{
    value_ = std::to_string(value);
    return *this;
}

Node& Node::append_child(const Node& child)
{
    children_.push_back(child);
    Node& node = children_.back();
    node.parent_ = this;
    return node;
}

} // end namespace
