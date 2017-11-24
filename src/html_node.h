#ifndef HTML_NODE_H
#define HTML_NODE_H

#include <map>
#include <vector>
#include <string>

namespace html
{ 

class Printer;
class Node
{
public:
    Node(const char* name) 
        :name_(name),parent_(nullptr)
    {}

    Node(const char* name,const char* value) 
        :name_(name),value_(value)
    {}

    Node(const char* name,int value) 
        :name_(name),value_(std::to_string(value)),parent_(nullptr)
    {}

    Node(const char* name,double value) 
        :name_(name),value_(std::to_string(value)),parent_(nullptr)
    {}

    Node()
    {}

    Node& attri(const char* name, const char* value)
    {
        attri_[name] = value;
        return *this;
    }

    Node& attri(const char* name, int value)
    {
        attri_[name] = std::to_string(value);
        return *this;
    }

    Node& attri(const char* name, double value)
    {
        attri_[name] = std::to_string(value);
        return *this;
    }

    Node& value (const char* value)
    {
        value_ = value;
        return *this;
    }

    Node& value (int value)
    {
        value_ = std::to_string(value);
        return *this;
    }

    Node& value (double value)
    {
        value_ = std::to_string(value);
        return *this;
    }

    Node& append_child(const Node& child)
    {
        children_.push_back(child);
        Node& node = children_.back();
        node.parent_ = this;
        return node;
    }

    friend Printer;

    Node* parent_;

    std::string name_;
    std::string value_;
    std::map<std::string, std::string> attri_;
    std::vector<Node> children_;
};
} // end namespace

#endif /* HTML_NODE_H */
