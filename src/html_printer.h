#ifndef HTML_PRINTER_H
#define HTML_PRINTER_H

#include <map>
#include <vector>
#include <string>
#include <ostream>
#include <assert.h>

namespace html
{ 
class Printer;
class V_Node;
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

    ~Node()
    {}

    void attri(const char* name, const char* value)
    {
        attri_[name] = value;
    }

    void attri(const char* name, int value)
    {
        attri_[name] = std::to_string(value);
    }

    void attri(const char* name, double value)
    {
        attri_[name] = std::to_string(value);
    }

    void value (const char* value)
    {
        value_ = value;
    }

    void value (int value)
    {
        value_ = std::to_string(value);
    }

    void value (double value)
    {
        value_ = std::to_string(value);
    }

    Node& append_child(const Node& child)
    {
        children_.push_back(child);
        Node& node = children_.back();
        node.parent_ = this;
        return node;
    }

    Node& operator>>(const Node& child)
    {
        append_child(child);
        return *this;
    }

    friend Printer;
protected:
    Node* parent_;

    std::string name_;
    std::string value_;
    std::map<std::string, std::string> attri_;
    std::vector<Node> children_;
};

class Printer 
{
public:
    Printer(std::ostream* out)
        :out_(out),indent_(0) {}
    ~Printer() {}


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

class document :public Node
{
public:
    document() :Node("html")
    {
        append_child("head").append_child("title");
        append_child("body");
    }
};

class div :public Node
{
public:
    div() :Node("div") { }
};

class img :public Node
{
public:
    img(const char* src) 
        :Node("img") 
    { 
        attri_[src] = src;
    }
};

class Td :public Node
{
public:
    Td(const char* text) 
        :Node("td",text) {}
};

class Tr :public Node
{
public:
    Tr() :Node("tr") {}
};

class Table :public Node
{
public:
    Table() :Node("table") {}
};

} // end namespace

#endif /* HTML_PRINTER_H */
