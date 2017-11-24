#include "html_vnode.h"

namespace html
{

VNode::VNode(const Node& node)
{
    sbling_node_.push_back(node);
}

VNode& VNode::operator+(const Node& node)
{
    sbling_node_.push_back(node);
    return *this;
}

} // end namespace

