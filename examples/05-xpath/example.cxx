
#include <xmlwrapp/xmlwrapp.h>

#include <iostream>

int main(int argc, char** argv)
{
    // parse an example file
    xml::tree_parser parser("example.xml");


    // instantiate an xpath-context
    // namespaces are registered here
    // with prefix and href.
    // see documentation for details
    xml::xpath::context ctxt(parser.get_document());

    // execute the query //child (finds all "child"-nodes)
    xml::xpath::node_set children = ctxt.evaluate("//child");

    // as you can see, node_set supports a standard set of
    // functions to iterator over the result-nodes.
    // a dereferenced iterator is a xml::node-reference.
    for (xml::xpath::node_set::iterator it = children.begin(); it != children.end(); ++it)
    {
        std::cout << "//child : " << it->get_name() << std::endl;
    }

    xml::xpath::node_set children_with_attribute = ctxt.evaluate("//child[@attrib=\"value\"]");
    
    for (xml::xpath::node_set::iterator it = children_with_attribute.begin(); it != children_with_attribute.end(); ++it)
    {
        std::cout << "//child[@attrib=\"value\"] : " << it->get_name() << std::endl;
    }



    ctxt.register_namespace("p", "http://example.com/a");

    xml::xpath::node_set subchildren = ctxt.evaluate(argv[1]);
//("/child/p:subchild");
    for (xml::xpath::node_set::iterator it = subchildren.begin(); it != subchildren.end(); ++it)
    {
        std::cout << "/child/p:subchild : " << it->get_name() << std::endl;
    }


}

