#ifndef HTML_VNODE_H
#define HTML_VNODE_H
#include "html_node.h"
#include <vector>

namespace html
{
    
class VNode
{
public:
    VNode(const Node& node);

    std::vector<Node> sbling_node_;
};

Node& operator>(Node& lhs,const Node& rhs);

VNode operator+(const Node& lhs, const Node& rhs);

VNode operator*(const Node& node,size_t n);

VNode& operator+(VNode& vnode,const Node& node);

VNode& operator+(const Node& node,VNode& vnode);

Node& operator>(Node& node,const VNode& vnode);

VNode& operator*(VNode& vnode,size_t n);

} // end namespace
#endif /* HTML_VNODE_H */
