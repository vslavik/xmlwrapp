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
 * This file contains the definition of the xml::node manipulation
 * functions.
**/

#ifndef _xmlwrapp_node_manip_h_
#define _xmlwrapp_node_manip_h_

// libxml includes
#include <libxml/tree.h>

namespace xmlwrapp {

    //####################################################################
    /** 
     * Insert a node somewhere in the child list of a parent node.
     *
     * @param parent The parent who's child list will be inserted into.
     * @param before Insert to_add before this node, or, if this node is 0 (null), insert at the end of the child list.
     * @param to_add The node to be copied and then inserted into the child list.
     * @return The new node that was inserted into the child list.
     * @author Peter Jones
    **/
    //####################################################################
    xmlNodePtr node_insert (xmlNodePtr parent, xmlNodePtr before, xmlNodePtr to_add);

    //####################################################################
    /** 
     * Replace a node with another one. The node being replaced will be
     * freed from memory.
     *
     * @param old_node The old node to remove and free.
     * @param new_node The new node to copy and insert where old node was.
     * @return The new node that was crated from copying new_node and inserted into the child list where old_node was.
     * @author Peter Jones
    **/
    //####################################################################
    xmlNodePtr node_replace (xmlNodePtr old_node, xmlNodePtr new_node);

    //####################################################################
    /** 
     * Erase a node from the child list, and then free it from memory.
     *
     * @param to_erase The node to remove and free.
     * @return The node that was after to_erase (may be 0 (null) if to_erase was the last node in the list)
     * @author Peter Jones
    **/
    //####################################################################
    xmlNodePtr node_erase (xmlNodePtr to_erase);
}
#endif
