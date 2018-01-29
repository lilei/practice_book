#include <string>

namespace parser
{
class Token
{
public:

    Token(int type,const char* text)
        :type_(type),text_(text)
    {
    }
    ~Token()
    {
    }

private:
    int type_;
    std::string text_;
};

}
