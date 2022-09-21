/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// xmlwrapp includes
#include "xmlwrapp/errors.h"

#include "node_manip.h"

// standard includes
#include <stdexcept>
#include <string>

// libxml includes
#include <libxml/tree.h>

namespace
{

// Check if two XML namespaces are the same. Both pointers must be non-null.
bool are_same(xmlNsPtr ns1, xmlNsPtr ns2)
{
    if ( ns1->type != ns2->type )
        return false;

    if ( xmlStrcmp(ns1->href, ns2->href) != 0 )
        return false;

    if ( ns1->prefix )
    {
        if ( !ns2->prefix || xmlStrcmp(ns1->prefix, ns2->prefix) != 0 )
            return false;
    }
    else
    {
        if ( ns2->prefix )
            return false;
    }

    return true;
}

// Helper: replaces the given namespace pointer (which is about to be freed)
// with another one for this node and all of its children, recursively, but
// without changing any other existing namespaces.
void replace_ns_recursively(xmlNodePtr node, xmlNsPtr old_ns, xmlNsPtr new_ns)
{
    if ( node->ns && node->ns != old_ns )
    {
        // This node uses a different namespace, so neither it nor its
        // children should inherit the parents one.
        return;
    }

    xmlSetNs(node, new_ns);

    for ( xmlNodePtr child = node->children; child; child = child->next )
        replace_ns_recursively(child, old_ns, new_ns);
}

// Make a copy of the given node with its contents and children which is meant
// to be added under the specified parent.
//
// The returned pointer is never null (but this function may throw) and must be
// freed with xmlFreeNode().
xmlNodePtr copy_node_under_parent(xmlNodePtr parent, xmlNodePtr orig_node)
{
    xmlNodePtr new_xml_node = xmlCopyNode(orig_node, 1);
    if ( !new_xml_node )
        throw std::bad_alloc();

    // Check that we really have a parent as this could also be xmlDoc
    // masquerading as xmlNode when inserting the root element itself.
    if ( parent->type == XML_DOCUMENT_NODE )
        return new_xml_node;

    // If there is no implicit namespace inherited from the parent, we're done.
    if ( !parent->ns )
        return new_xml_node;

    // Check if any of the namespace definitions on the new node is not
    // redundant with the namespace implicitly inherited from the parent:
    // this happens whenever the original node is in a namespace, as
    // xmlCopyNode() creates an artificial definition for it at the node
    // level in this case and we want to remove this redundant definition
    // if possible.
    xmlNsPtr nsToBeFreed = NULL;
    if ( new_xml_node->ns && are_same(new_xml_node->ns, parent->ns) )
    {
        // We can indeed remove this namespace definition, so do it.
        for ( xmlNsPtr* pns = &new_xml_node->nsDef; *pns; pns = &(*pns)->next )
        {
            if ( *pns == new_xml_node->ns )
            {
                // Unlink the node from the linked list but don't free it yet,
                // we can't do it as long as this namespace definition is still
                // (potentially) used by this node children, just remember to
                // do it later.
                nsToBeFreed = *pns;
                *pns = (*pns)->next;

                break;
            }
        }
    }

    // The new node and all its children should inherit the namespace of
    // its parent, unless any of them already have a custom namespace, for
    // consistency with how tree construction works in libxml2.
    replace_ns_recursively(new_xml_node, nsToBeFreed, parent->ns);

    // Now that children namespaces pointing to the namespace definition in
    // this node have been replaced with the new namespace pointers too, we can
    // free the old definition.
    if ( nsToBeFreed )
        xmlFreeNs(nsToBeFreed);

    return new_xml_node;
}

} // anonymous namespace

xmlNodePtr
xml::impl::node_insert(xmlNodePtr parent, xmlNodePtr before, xmlNodePtr to_add)
{
    xmlNodePtr const new_xml_node = copy_node_under_parent(parent, to_add);

    if ( before == 0 )
    {
        // insert at the end of the child list
        if ( xmlAddChild(parent, new_xml_node) == 0 )
        {
            xmlFreeNode(new_xml_node);
            throw xml::exception("failed to insert xml::node; xmlAddChild failed");
        }
    }
    else
    {
        if ( xmlAddPrevSibling(before, new_xml_node) == 0 )
        {
            xmlFreeNode(new_xml_node);
            throw xml::exception("failed to insert xml::node; xmlAddPrevSibling failed");
        }
    }

    return new_xml_node;
}


xmlNodePtr
xml::impl::node_replace(xmlNodePtr old_node, xmlNodePtr new_node)
{
    xmlNodePtr const copied_node = copy_node_under_parent(old_node->parent, new_node);

    // hack to see if xmlReplaceNode was successful
    xmlDocPtr temp_to_test_replace_success = xmlNewDoc((const xmlChar *)("1.0"));
    if ( !temp_to_test_replace_success ) {
        xmlFreeNode(copied_node);
        throw std::bad_alloc();
    }

    copied_node->doc = temp_to_test_replace_success;
    xmlReplaceNode(old_node, copied_node);

    if ( copied_node->doc == temp_to_test_replace_success )
    {
        xmlFreeDoc(temp_to_test_replace_success);
        xmlFreeNode(copied_node);
        throw xml::exception("failed to replace xml::node; xmlReplaceNode() failed");
    }

    xmlFreeDoc(temp_to_test_replace_success);
    xmlFreeNode(old_node);
    return copied_node;
}


xmlNodePtr xml::impl::node_erase(xmlNodePtr to_erase)
{
    if ( !to_erase )
        throw std::invalid_argument("erasing invalid node");

    xmlNodePtr after = to_erase->next;

    xmlUnlinkNode(to_erase);
    xmlFreeNode(to_erase);

    return after;
}

void xml::impl::node_set_ns_recursively(xmlNodePtr node, xmlNsPtr ns)
{
    replace_ns_recursively(node, NULL, ns);
}
