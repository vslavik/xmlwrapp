#!/usr/bin/perl
###########################################################################
# Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
# All Rights Reserved
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name of the Author nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
###########################################################################

use strict;
use lib qw(../harness);
use harness;

my $good_xml = qq(<root><a><b><c><d><e><f><g/></f></e></d></c></b></a></root>);
my $bad_xml  = qq(<root><a><b></c></d></root>);

my $good_result;
my $actual_result;

my @xmllist = qw(root a b c d e f g);
for (my $i=0; $i<@xmllist; ++$i) {
    $good_result .= ' ' x ($i * 4);
    $good_result .= $xmllist[$i] . "\n";
}

unless (open(TMP, ">good.xml")) {
    print STDERR "$0: can't create good.xml: $!\n";
    exit 1;
}

print TMP "$good_xml\n";
close TMP;

unless (open(TMP, ">bad.xml")) {
    print STDERR "$0: can't create bad.xml: $!\n";
    exit 1;
}

print TMP "$bad_xml\n";
close TMP;

my $test = harness->new("xml::tree_parser");
runtests();
unlink("good.xml", "bad.xml");

sub runtests {
    ###########################################################################
    $test->regression("parse good file (01)", "./test_tree-01 good.xml", $good_result);
    ###########################################################################
    $test->regression("parse bad file (02)", "./test_tree-02 bad.xml", "exception\n");
    ###########################################################################
    $test->regression("parse bad file (03)", "./test_tree-03 bad.xml", "failed\n");
    ###########################################################################
    $test->regression("constructor (const char*) good xml (04)", "./test_tree-04 '$good_xml'", $good_result);
    ###########################################################################
    $test->regression("constructor (const char*) bad xml (05)", "./test_tree-05 '$bad_xml'", "exception\n");
    ###########################################################################
    $test->regression("constructor (const char*) bad xml (06)", "./test_tree-06 '$bad_xml'", "failed\n");
    ###########################################################################
}
