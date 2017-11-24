#ifndef HTML_VNODE_H
#define HTML_VNODE_H
#include "html_node.h"
#include <vector>

namespace html
{
    
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
#endif /* HTML_VNODE_H */
