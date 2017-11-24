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

class Printer 
{
public:
    Printer(std::ostream* out)
        :out_(out),indent_(0) {}

    Printer() {}

    Printer& operator<<(const Node& node)
    {
        begin_tag(node.name_);
        for (const auto& kv:node.attri_)
        {
            attri(kv.first, kv.second);
        }
        value(node.value_);

        for (const auto& node:node.children_)
        {
            *this << node;
        }

        end_tag(node.name_);

        return *this;
    }

private:
    void begin_tag(const std::string& tag_name)
    {
        *out_ << std::endl << blanks() << "<" << tag_name;
        indent();
    }

    void attri(const std::string& attr_name,const std::string& attr_value)
    {
        *out_ << " " << attr_name << " = \"" << attr_value << "\"";
    }

    void value(const std::string& value)
    {
        *out_ << ">";
        if (!value.empty())
        {
            *out_ << std::endl << blanks() << value;
        }
    }

    void end_tag(const std::string& tag_name)
    {
        outdent();
        *out_ << std::endl << blanks() << "</" << tag_name << ">";
    }

    std::string blanks()
    {
        std::string blank;
        for (int i = 0;i < indent_;i++)
        {
            blank.append("  ");
        }
        return blank;
    }

    void indent() 
    {
        ++indent_;
    }

    void outdent() 
    {
        --indent_;
        if (0 == indent_)
        {
            indent_ = 0;
        }
    }

    std::ostream* out_;
    int indent_;
};

class Html  :public Node { public: Html()  :Node("html")  {} };
class Head  :public Node { public: Head()  :Node("head")  {} };
class Body  :public Node { public: Body()  :Node("body")  {} };
class Title :public Node { public: Title() :Node("title") {} };
class Div   :public Node { public: Div()   :Node("div")   {} };
class Img   :public Node { public: Img()   :Node("img")   {} };
class Table :public Node { public: Table() :Node("table") {} };
class Tr    :public Node { public: Tr()    :Node("tr")    {} };
class Td    :public Node { public: Td()    :Node("td")    {} };
class Span  :public Node { public: Span()  :Node("span")  {} };
class Hr    :public Node { public: Hr()    :Node("hr")    {} };

} // end namespace

#endif /* HTML_NODE_H */
