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

my $test = harness->new("xml::document");
runtests();

###########################################################################
sub runtests {
    my $actual_result;
    my $good_result;

    ###########################################################################
    $test->regression("iteration (01)", "./test_document-01 data/01.xml", "data/01.out");
    ###########################################################################
    foreach (qw(02 03 04 05 06 07 08)) {
	$test->regression("constructors ($_)", "./test_document-$_", "data/$_.out");
    }
    ###########################################################################
    foreach (qw(09 10)) {
	$test->regression("version ($_)", "./test_document-$_ data/$_.xml", "data/$_.out");
    }
    ###########################################################################
    foreach (qw(11 12)) {
	$test->regression("encoding ($_)", "./test_document-$_ data/$_.xml", "data/$_.out");
    }
    ###########################################################################
    foreach (qw(a b)) {
	$test->regression("standalone (13$_)", "./test_document-13 data/13$_.xml", "data/13$_.out");
    }
    ###########################################################################
    $test->regression("xinclusion (14)", "./test_document-14 data/14.xml", "data/14.out");
    ###########################################################################
    $test->regression("save_to_file (15a)", "./test_document-15 /dev/stdout 0", "data/15.out");
    $test->regression("save_to_file (15b)", "./test_document-15 /dev/stdout 9 | gunzip -c", "data/15.out");
    ###########################################################################
    $test->regression("size (16)", "./test_document-16", "data/16.out");
    ###########################################################################
    $test->regression("insert (17)", "./test_document-17", "data/17.out");
    ###########################################################################
    foreach (qw(a b c)) {
	$test->regression("insert (18$_)", "./test_document-18 $_", "data/18$_.out");
    }
    ###########################################################################
    $test->regression("replace (19)", "./test_document-19", "data/19.out");
    ###########################################################################
    foreach (qw(a b)) {
	$test->regression("replace (20$_)", "./test_document-20 $_", "data/20.out");
    }
    ###########################################################################
    $test->regression("erase (21)", "./test_document-21", "data/21.out");
    ###########################################################################
    $test->regression("erase (22)", "./test_document-22", "data/22.out");
    ###########################################################################
}
###########################################################################
