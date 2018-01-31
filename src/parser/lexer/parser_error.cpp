#include "parser_error.h"

std::ostream& operator<<(std::ostream& out,const ParserError& e)
{
    if (e.char_error_)
    {
        out << "invalid character: " << e.invalid_char_ << std::endl;
    }
    else
    {
        out << "expecting " << e.expect_ << ";found " << e.found_ << std::endl;
    }
    return out;
}

