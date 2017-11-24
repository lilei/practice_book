#ifndef HTML_ELEMENT_H
#define HTML_ELEMENT_H
#include "html_node.h"

namespace html
{ 

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

#endif /* HTML_ELEMENT_H */
