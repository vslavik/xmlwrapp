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

my $test = harness->new("xml::node");
runtests();

###########################################################################
sub runtests {
    ###########################################################################
    $test->regression("constructor (01)", "./test_node-01", "data/01.xml");
    ###########################################################################
    foreach my $i (qw(02a 02b 02c 02d)) {
	$test->regression("find ($i)", "./test_node-$i data/02.xml", "data/$i.out");
    }
    ###########################################################################
    foreach my $i (qw(03a 03b)) {
	$test->regression("replace ($i)", "./test_node-$i data/03.xml", "data/$i.out");
    }
    ###########################################################################
    foreach my $i (qw(04a 04b)) {
	$test->regression("erase ($i)", "./test_node-$i data/04.xml", "data/$i.out");
    }
    ###########################################################################
    foreach my $i (qw(05a 05b 05c 05d)) {
	$test->regression("insert ($i)", "./test_node-$i data/05.xml", "data/$i.out");
    }
    ###########################################################################
    foreach my $i (qw(06a 06b 06c)) {
	$test->regression("get_type ($i)", "./test_node-06 data/$i.xml", "data/$i.out");
    }
    ###########################################################################
    foreach my $i (qw(07a 07b)) {
	$test->regression("sort ($i)", "./test_node-07 data/$i.xml child order", "data/$i.out");
    }
    ###########################################################################
    foreach my $i (qw(08a)) {
	$test->regression("sort_fo ($i)", "./test_node-08 data/$i.xml", "data/$i.out");
    }
    ###########################################################################
    $test->regression("cdata (09)", "./test_node-09", "data/09.out");
    ###########################################################################
    $test->regression("commet (10)", "./test_node-10", "data/10.out");
    ###########################################################################
    $test->regression("pi (11)", "./test_node-11", "data/11.out");
    ###########################################################################
    $test->regression("size (12)", "./test_node-12", "data/12.out");
    ###########################################################################
    $test->regression("empty (13)", "./test_node-13", "data/13.out");
    ###########################################################################
}
###########################################################################
