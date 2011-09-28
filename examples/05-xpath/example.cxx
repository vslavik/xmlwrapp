
#include <xmlwrapp/xmlwrapp.h>
#include <xmlwrapp/xpath.h>

#include <iostream>

int main()
{
    xml::tree_parser parser("example.xml");
    xml::xpath::context ctxt(parser.get_document());

    xml::xpath::node_set ns = ctxt.evaluate("//child");

    std::cout << "node_set.count = " << ns.count() << std::endl;

    for (xml::xpath::node_set::iterator it = ns.begin(); it != ns.end(); ++it)
    {
        std::cout << it->get_name() << std::endl;
    }
}

