
#include <xmlwrapp/xmlwrapp.h>

#include <iostream>

int main(int argc, char *argv[])
{
    // parse an example file
    xml::tree_parser parser("example.xml");


    // instantiate an xpath-context
    // namespaces are registered here
    // with prefix and href.
    // see documentation for details
    xml::xpath_context ctxt(parser.get_document());

    // execute the query //child (finds all "child"-nodes)
    xml::const_nodes_view children = ctxt.evaluate("//child");

    // as you can see, node_set can be iterated over as any range.
    for (auto const& child : children)
    {
        std::cout << "//child : " << child.get_name() << std::endl;
    }

    for (auto const& child_with_attr : ctxt.evaluate("//child[@attrib=\"value\"]"))
    {
        std::cout << "//child[@attrib=\"value\"] : " << child_with_attr.get_name() << std::endl;
    }



    ctxt.register_namespace("p", "http://example.com/a");

    const char *xpath = argc > 1 ? argv[1] : "/child/p:subchild";
    for (auto const& subchild : ctxt.evaluate(xpath))
    {
        std::cout << "/child/p:subchild : " << subchild.get_name() << std::endl;
    }


    return 0;
}

