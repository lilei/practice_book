#ifndef HTML_PRINTER_H
#define HTML_PRINTER_H

#include "html_node.h"
#include <ostream>

namespace html
{ 
class Printer 
{
public:
    Printer(std::ostream* out);
    ~Printer();

    Printer& operator<<(const Node& node);

private:
    void begin_tag(const std::string& tag_name);

    void attri(const std::string& attr_name, const std::string& attr_value);
    
    void value(const std::string& value);

    void end_tag(const std::string& tag_name);

    std::string blanks();

    void indent();

    void outdent();

    std::ostream* out_;
    int indent_;
};

} // end namespace

#endif /* HTML_PRINTER_H */
