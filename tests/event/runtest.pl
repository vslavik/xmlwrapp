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

my $test = harness->new("xml::event_parser");
runtests();

###########################################################################
sub runtests {
    my $actual_result;
    my $good_result;

    ###########################################################################
    $test->start("test_event-01");
    $actual_result = `./test_event-01 data/01.xml 2>&1`;

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
    foreach my $cbname (qw(start_element end_element text processing_instruction comment cdata)) {
	$test->start("test_event-02 ($cbname)");
	$actual_result = `./test_event-02 data/01.xml $cbname 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    chomp($actual_result);
	    
	    if ($actual_result ne $cbname) {
		$test->fail("expected to see $cbname, but saw $actual_result instead");
	    } else {
		$test->pass();
	    }
	}

	$test->start("test_event-02 ($cbname throw)");
	$actual_result = `./test_event-02 data/01.xml $cbname throw 2>&1`;

	if ($? != 0) {
	    $test->fail("test process returned $?");
	} else {
	    chomp($actual_result);

	    if ($actual_result ne "$cbname throw") {
		$test->fail("expected to see '$cbname throw', be saw '$actual_result' instead");
	    } else {
		$test->pass();
	    }
	}
    }
    ###########################################################################
    $test->start("test_event-03");
    $actual_result = `./test_event-03 data/01.xml 2>&1`;

    if ($? != 0) {
	$test->fail("test process retured $?");
    } else {
	if ($actual_result =~ /inside two_dot_two/) {
	    $test->pass();
	} else {
	    $test->fail("expected to see 'inside two_dot_two' in result");
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
