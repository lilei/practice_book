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

class VNode
{
public:
    VNode(const Node& node)
    {
        sbling_node_.push_back(node);
    }
    std::vector<Node> sbling_node_;
};

Node& operator>(Node& lhs, const Node& rhs)
{
    const Node *node = &rhs;
    while (nullptr != node->parent_)
    {
        node = node->parent_;
    }
    lhs.append_child(*node);
    return lhs.children_.back();
}


VNode& operator+(VNode& vnode, const Node& node)
{
    vnode.sbling_node_.push_back(node);
    return vnode;
}

VNode operator+(const Node& lhs, const Node& rhs)
{
    VNode vnode(lhs);
    vnode + rhs;
    return vnode;
}

VNode& operator+(const Node& node,VNode& vnode)
{
    vnode.sbling_node_.insert(vnode.sbling_node_.begin(),node);
    return vnode;
}

VNode operator*(const Node& node, size_t n)
{
    VNode vnode(node);
    for (size_t i = 0;i < n -1;i++)
    {
        vnode + node;
    }
    return vnode;
}


html::Node& operator>(Node& node, const VNode& vnode)
{
    for (auto sbling:vnode.sbling_node_)
    {
        node > sbling;
    }
    return node;
}

VNode& operator*(VNode& vnode, size_t n)
{
    std::vector<Node> nodes = vnode.sbling_node_;
    std::vector<Node> &sblings = vnode.sbling_node_;
    for (size_t i = 0; i < n - 1; i++)
    {
        std::copy(nodes.begin(), nodes.end(), std::back_inserter(sblings));
    }
    return vnode;
}
} // end namespace

#endif /* HTML_NODE_H */
