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
    my $actual_result;
    my $good_result;

    ###########################################################################
    $test->start("test_node-01");
    $actual_result = `./test_node-01 2>&1`;

    if ($? != 0) {
	$test->fail("test process returned $?");
    } else {
	$good_result = slurp_file("data/01.xml");

	if ($actual_result ne $good_result) {
	    $test->fail('output did not match expected value');
	} else {
	    $test->pass();
	}
    }
    ###########################################################################
    foreach my $i (qw(02a 02b 02c 02d)) {
	$test->start("test_node-$i");
	$actual_result = `./test_node-$i data/02.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$i.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach my $i (qw(03a 03b)) {
	$test->start("test_node-$i");
	$actual_result = `./test_node-$i data/03.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$i.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach my $i (qw(04a 04b)) {
	$test->start("test_node-$i");
	$actual_result = `./test_node-$i data/04.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$i.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach my $i (qw(05a 05b 05c 05d)) {
	$test->start("test_node-$i");
	$actual_result = `./test_node-$i data/05.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$i.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach my $i (qw(06a 06b 06c)) {
	$test->start("test_node-$i");
	$actual_result = `./test_node-06 data/$i.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/$i.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach my $i (qw(a b)) {
	$test->start("xml::node::sort ($i)");
	$actual_result = `./test_node-07 data/07$i.xml child order 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/07$i.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    foreach my $i (qw(a)) {
	$test->start("xml::node::sort_fo ($i)");
	$actual_result = `./test_node-08 data/08$i.xml 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    $good_result = slurp_file("data/08$i.out");

	    if ($actual_result ne $good_result) {
		$test->fail('output did not match expected value');
	    } else {
		$test->pass();
	    }
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
