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

    VNode& operator+(const Node& node);

    std::vector<Node> sbling_node_;
};

} // end namespace
#endif /* HTML_VNODE_H */
