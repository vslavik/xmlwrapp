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

my $test = harness->new("xslt::stylesheet");
runtests();

###########################################################################
sub runtests {
    my $actual_result;
    my $good_result;

    ###########################################################################
    foreach (['a', 1], ['b', 0], ['c', 1]) {
	$test->start("constructor $_->[0]");
	$actual_result = `./test_xslt-01 data/01$_->[0].xsl 2>&1`;

	if ($_->[1] == 0 and $? != 0) {
	    $test->fail("expected exit status of 0 but got $?");
	} elsif ($_->[1] != 0 and $? == 0) {
	    $test->fail("expected exit other than 0 but got $?");
	} else {
	    $test->pass();
	}
    }
    ###########################################################################
    $test->start("apply (2)");
    $actual_result = `./test_xslt-02 data/02a.xsl data/02a.xml 2>&1`;
    $good_result = slurp_file("data/02a.out");

    if ($? != 0) {
	$test->fail("expected exit status of 0 but got $?");
    } elsif ($actual_result ne $good_result) {
	$test->fail("output did not match expected result");
    } else {
	$test->pass();
    }
    ###########################################################################
    $test->start("apply (3)");
    $actual_result = `./test_xslt-03 data/03a.xsl data/03a.xml "'bar'" 2>&1`;
    $good_result = slurp_file("data/03a.out");

    if ($? != 0) {
	$test->fail("expected exit status of 0 but got $?");
    } elsif ($actual_result ne $good_result) {
	$test->fail("output did not match expected result");
    } else {
	$test->pass();
    }
    ###########################################################################
    $test->start("apply (4)");
    $actual_result = `./test_xslt-04 data/04a.xsl data/04a.xml 2>&1`;
    $good_result = slurp_file("data/04a.out");

    if ($? != 0) {
	$test->fail("expected exit status of 0 but got $?");
    } elsif ($actual_result ne $good_result) {
	$test->fail("output did not match expected result");
    } else {
	$test->pass();
    }
    ###########################################################################
    $test->start("apply (5)");
    $actual_result = `./test_xslt-05 data/05a.xsl data/05a.xml "'bar'" 2>&1`;
    $good_result = slurp_file("data/05a.out");

    if ($? != 0) {
	$test->fail("expected exit status of 0 but got $?");
    } elsif ($actual_result ne $good_result) {
	$test->fail("output did not match expected result");
    } else {
	$test->pass();
    }
    ###########################################################################
}
###########################################################################
sub slurp_file {
    my $filename = shift;
    my $out;


    if (not open(SF, $filename)) {
	print STDERR "\n\n$0: failed to open $filename: $!\n";
	exit 1;
    }

    my $save = $/; $/=undef; $out = <SF>; $/=$save;
    close SF;

    return $out;
}
###########################################################################
