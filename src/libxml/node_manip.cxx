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

    // The new node should inherit the namespace of its parent, unless it
    // already has a custom namespace, for consistency with how tree
    // construction works in libxml2.
    if ( !new_xml_node->ns )
    {
        // Check that we really have a parent as this could also be xmlDoc
        // masquerading as xmlNode when inserting the root element itself.
        if ( parent->type != XML_DOCUMENT_NODE )
            new_xml_node->ns = parent->ns;
    }

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
    copied_node->doc = reinterpret_cast<xmlDocPtr>(old_node);
    xmlReplaceNode(old_node, copied_node);

    if ( copied_node->doc == reinterpret_cast<xmlDocPtr>(old_node) )
    {
        xmlFreeNode(copied_node);
        throw xml::exception("failed to replace xml::node; xmlReplaceNode() failed");
    }

    xmlFreeNode(old_node);
    return copied_node;
}


xmlNodePtr xml::impl::node_erase(xmlNodePtr to_erase)
{
    xmlNodePtr after = to_erase->next;

    xmlUnlinkNode(to_erase);
    xmlFreeNode(to_erase);

    return after;
}
