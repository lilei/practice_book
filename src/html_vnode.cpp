#include "html_vnode.h"

namespace html
{

VNode::VNode(const Node& node)
{
    sbling_node_.push_back(node);
}

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

VNode operator+(const Node& lhs, const Node& rhs)
{
    VNode vnode(lhs);
    vnode + rhs;
    return vnode;
}

VNode& operator+(VNode& vnode, const Node& node)
{
    vnode.sbling_node_.push_back(node);
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

} // end namespace

