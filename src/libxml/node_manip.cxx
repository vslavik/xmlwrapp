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

/** @file
 * This file contains the implementation of the xml::node manipulation
 * functions.
**/

// xmlwrapp includes
#include "node_manip.h"

// standard includes
#include <stdexcept>

// libxml includes
#include <libxml/tree.h>

//####################################################################
xmlNodePtr xmlwrapp::node_insert (xmlNodePtr parent, xmlNodePtr before, xmlNodePtr to_add) {
    xmlNodePtr new_xml_node =  xmlCopyNode(to_add, 1);
    if (!new_xml_node) throw std::bad_alloc();

    if (before == 0) { // insert at the end of the child list
	if (xmlAddChild(parent, new_xml_node) == 0) {
	    xmlFreeNode(new_xml_node);
	    throw std::runtime_error("failed to insert xml::node; xmlAddChild failed");
	}
    } else {
	if (xmlAddPrevSibling(before, new_xml_node) == 0) {
	    xmlFreeNode(new_xml_node);
	    throw std::runtime_error("failed to insert xml::node; xmlAddPrevSibling failed");
	}
    }

    return new_xml_node;
}
//####################################################################
xmlNodePtr xmlwrapp::node_replace (xmlNodePtr old_node, xmlNodePtr new_node) {
    xmlNodePtr copied_node =  xmlCopyNode(new_node, 1);
    if (!copied_node) throw std::bad_alloc();

    // hack to see if xmlReplaceNode was successful
    copied_node->doc = reinterpret_cast<xmlDocPtr>(old_node);
    xmlReplaceNode(old_node, copied_node);

    if (copied_node->doc == reinterpret_cast<xmlDocPtr>(old_node)) {
	xmlFreeNode(copied_node);
	throw std::runtime_error("failed to replace xml::node; xmlReplaceNode() failed");
    }

    xmlFreeNode(old_node);
    return copied_node;
}
//####################################################################
xmlNodePtr xmlwrapp::node_erase (xmlNodePtr to_erase) {
    xmlNodePtr after = to_erase->next;

    xmlUnlinkNode(to_erase);
    xmlFreeNode(to_erase);

    return after;
}
//####################################################################
