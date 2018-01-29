#include <string>

namespace parser
{

class Error
{
public:

enum Type
{
    INVALID_CHAR
};

Error(Type type,char c) 
{
    if (INVALID_CHAR == type)
    {
        error_string_ = "invalid character: ";
        error_string_.push_back(c);
    }
}

const std::string& to_string() const
{
    return error_string_;
};


private:
    std::string error_string_;
};


}