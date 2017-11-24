#ifndef HTML_NODE_H
#define HTML_NODE_H

#include <map>
#include <vector>
#include <string>
#include <memory>

namespace html
{ 

class Printer;
class VNode;
class Node
{
public:
    Node(const char* name);

    Node(const char* name, const char* value);

    Node(const char* name, int value);

    Node(const char* name, double value);

    ~Node();

    Node& attri(const char* name, const char* value);

    Node& attri(const char* name, int value);

    Node& attri(const char* name, double value);

    Node& value(const char* value);

    Node& value(int value);

    Node& value(double value);

    Node& append_child(const Node& child);

    Node& operator>(const Node& child);

    Node& operator>(std::shared_ptr<VNode> vnode);

    std::shared_ptr<VNode> operator+(const Node& sibling);

    std::shared_ptr<VNode> operator*(size_t n);

    friend Printer;
protected:
    Node* parent_;

    std::string name_;
    std::string value_;
    std::map<std::string, std::string> attri_;
    std::vector<Node> children_;
};
} // end namespace

#endif /* HTML_NODE_H */
