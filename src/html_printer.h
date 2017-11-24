#ifndef HTML_PRINTER_H
#define HTML_PRINTER_H

#include "html_node.h"
#include <ostream>

namespace html
{ 
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

} // end namespace

#endif /* HTML_PRINTER_H */
