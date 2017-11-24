#include "html_printer.h"

namespace html
{ 
Printer::Printer(std::ostream* out)
    :out_(out),indent_(0) {}

Printer::~Printer() {}


Printer& Printer::operator<<(const Node& node)
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

void Printer::begin_tag(const std::string& tag_name)
{
    *out_ << std::endl << blanks() << "<" << tag_name;
    indent();
}

void Printer::attri(const std::string& attr_name,const std::string& attr_value)
{
    *out_ << " " << attr_name << " = \"" << attr_value << "\"";
}

void Printer::value(const std::string& value)
{
    *out_ << ">";
    if (!value.empty())
    {
        *out_ << std::endl << blanks() << value;
    }
}

void Printer::end_tag(const std::string& tag_name)
{
    outdent();
    *out_ << std::endl << blanks() << "</" << tag_name << ">";
}

std::string Printer::blanks()
{
    std::string blank;
    for (int i = 0;i < indent_;i++)
    {
        blank.append("  ");
    }
    return blank;
}

void Printer::indent() 
{
    ++indent_;
}

void Printer::outdent() 
{
    --indent_;
    if (0 == indent_)
    {
        indent_ = 0;
    }
}

} // end namespace

