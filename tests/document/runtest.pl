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
    $test->start("iteration test_document-01");
    $actual_result = `./test_document-01 data/01.xml 2>&1`;

    if ($? != 0) {
	$test->fail("test process returned $?");
    } else {
	$good_result = slurp_file("data/01.out");

	if ($actual_result ne $good_result) {
	    $test->fail('output did not match expected value');
	} else {
	    $test->pass();
	}
    }
    ###########################################################################
    foreach (qw(02 03 04 05 06 07 08)) {
	$test->start("constructors test_document-$_");
	$actual_result = `./test_document-$_ 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$_.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach (qw(09 10)) {
	$test->start("version test_document-$_");
	$actual_result = `./test_document-$_ data/$_.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$_.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach (qw(11 12)) {
	$test->start("encoding test_document-$_");
	$actual_result = `./test_document-$_ data/$_.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$_.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach (qw(a b)) {
	$test->start("standalone test_document-13$_");
	$actual_result = `./test_document-13 data/13$_.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/13$_.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    $test->start("xinclusion test_document-14");
    $actual_result = `./test_document-14 data/14.xml 2>&1`;

    if ($? != 0) {
	$test->fail("test process returned $?");
    } else {
	$good_result = slurp_file("data/14.out");

	if ($actual_result ne $good_result) {
	    $test->fail('output did not match expected value');
	} else {
	    $test->pass();
	}
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
