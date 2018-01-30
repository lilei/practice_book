#include "list_token.h"

std::ostream& operator<<(std::ostream& out, Token token)
{
    out << "<'" << token.text_ << "'," << token_name_[token.type_] << ">" << std::endl;
    return out;
}

