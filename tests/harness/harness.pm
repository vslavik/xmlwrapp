package harness;
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
#
# Test harness code. For generating screen output and XML output.
# $Id: harness.pm,v 1.4 2003/08/18 04:33:06 pjones Exp $
#
# To use:
#
#   $mytest = harness->new("Name of My test");
#
#   $mytest->start("sub test one");
#
#   $mytest->pass;
#
#   -- or --
#
#   $mytest->fail("reason the test failed")
#
###########################################################################
use strict;
###########################################################################
my $color_red	    = '';
my $color_green	    = '';
my $color_blue	    = '';
my $color_magenta   = '';
my $color_reset	    = '';
my $wchar	    = 78;
###########################################################################
eval
{
    require Term::ANSIColor;
    import Term::ANSIColor;

    if (-t STDOUT) {
	$color_red	= color("red");
	$color_green    = color("green");
	$color_blue	= color("blue");
	$color_magenta  = color("magenta");
	$color_reset    = color("reset");
    }

    require Term::ReadKey;
    import Term::ReadKey;
    ($wchar, undef, undef, undef) = GetTerminalSize();
    $wchar -= 4; # Term::ReadKey can sometimes be off by a few columns
};
###########################################################################
sub new
{
    my $class = shift;
    my $test_name = shift;

    if (not defined $test_name) {
	die "missing test name paramenter to harness->new()";
    }

    my $self = bless({ 
	'name'		=> $test_name ,
	'xml'		=> ($ENV{'HARNESS_XML'} ? 1 : 0),
	'time'		=> time,
	'tests'		=> 0,
	'pass'		=> 0,
	'fail'		=> 0,
	
	'die_on_fail'	=> 0,
	'die_on_end'	=> 1,
    }, $class);

    if ($ENV{'HARNESS_XML_FILE'}) {
	$self->{'xml'} = 1;

	unless (open(XMLOUT, ">>$ENV{HARNESS_XML_FILE}")) {
	    print STDERR "harness: can't open XML output file $ENV{HARNESS_XML_FILE}: $!\n";
	    exit 1;
	}

	select XMLOUT;

	$self->die_on_fail(0);
	$self->die_on_end(0);
    }

    if ($self->xml) {
	print qq(<test name="$self->{name}" time="$self->{time}">\n);
    } else {
	print "${color_magenta}$test_name ";
	print '-' x ($wchar - length($test_name) - 1);
	print "${color_reset}\n";
    }

    return $self;
}
###########################################################################
sub start 
{
    my $self = shift;
    my $test_name = shift;

    if (not defined $test_name) {
	die "missing test name parameter to harness->start()";
    }

    $self->{'last_test_name'} = $test_name;

    if (not $self->xml) {
	print "$test_name ";
	$self->dots;
    }

    ++$self->{'tests'};
}
###########################################################################
sub pass 
{
    my $self = shift;

    if ($self->xml) {
	print qq(<item name="$self->{last_test_name}" state="pass"/>\n);
    } else {
	print " [ ${color_green}OKAY${color_reset} ]\n";
    }

    ++$self->{'pass'};
}
###########################################################################
sub fail 
{
    my $self = shift;
    my $info = shift;

    if (not defined $info) {
	die "missing failure info paramenter to harness->fail()";
    }

    if ($self->xml) {
	print qq(<item name="$self->{last_test_name}" state="fail"><![CDATA[$info]]></item>\n);
    } else {
	print " [ ${color_red}FAIL${color_reset} ]\n";
	print "${color_red}", ('-' x $wchar), "${color_reset}\n";
	print "$info";
	print "\n" unless substr($info, -1, 1) eq "\n";
	print "${color_red}", ('-' x $wchar), "${color_reset}\n";

	if ($self->{'die_on_fail'}) {
	    print STDERR "test failure for $self->{last_test_name}: $info\n";
	    exit 1;
	}
    }

    ++$self->{'fail'};
}
###########################################################################
sub regression
{
    my $self	    = shift;
    my $name	    = shift; # name of test passed to $self->start()
    my $run_test    = shift; # sub or program to run to generate output to test
    my $run_result  = shift; # sub, program or file to use for "good" results

    $self->start($name);

    my $result;
    my $good;

    if (ref $run_test and ref $run_test eq 'CODE') {
	$result = $run_test->($name);
    } else {
	$result = `$run_test 2>&1`;

	if ($? != 0) {
	    $self->fail("test program $run_test failed with exit code $?");
	    return;
	}
    }

    if (ref $run_result and ref $run_result eq 'CODE') {
	$good = $run_result->($name);
    } elsif (ref $run_result and ref $run_result eq 'Regexp') {
	if ($result =~ $run_result) {
	    $result = $good = 1;
	} else {
	    $good = "=~ $run_result";
	}
    } elsif (-e $run_result and not -x $run_result) {
	$good = $self->slurp_file($run_result);
    } elsif (-e $run_result and -x $run_result) {
	$good = `$run_result 2>&1`;

	if ($? != 0) {
	    $self->fail("failed to load regression data from command $run_result");
	    return;
	}
    } else {
	$good = $run_result;
    }

    if ($result eq $good) {
	$self->pass();
    } else {
	$self->fail("regression failed\nEXPECTED: $good\nRECEIVED: $result")
    }
}
###########################################################################
sub run_test_exit_status
{
    my $self		    = shift;
    my $name		    = shift; # the name of the test
    my $run_program	    = shift; # the name of the program to run
    my $expect_exit_status  = shift; # what the exit status should be

    $self->start($name);
    my $null = `$run_program 2>&1`;

    if ($? != 0) { $? >>= 8; }

    if ($? == $expect_exit_status) {
	$self->pass();
    } else {
	$self->fail("bad run of $run_program, expected exit with $expect_exit_status but got $?");
    }
}
###########################################################################
sub die_on_fail 
{
    my $self = shift;
    my $state = shift || 1;
    $self->{'die_on_fail'} = $state;
}
###########################################################################
sub die_on_end 
{
    my $self = shift;
    my $state = shift || 1;
    $self->{'die_on_end'} = $state;
}
###########################################################################
sub xml 
{ 
    return $_[0]->{'xml'}; 
}
###########################################################################
sub dots 
{ 
    print '.' x ($wchar - length($_[0]->{'last_test_name'}) - 8) 
}
###########################################################################
sub slurp_file 
{
    my $self = shift;
    my $filename = shift;
    my $out;

    if (not open(SF, $filename)) {
	print STDERR "\n\n$0: failed to open $filename: $!\n";
	exit 1 if $self->{'die_on_fail'};
    }

    my $save = $/; $/=undef; $out = <SF>; $/=$save;
    close SF;

    return $out;
}
###########################################################################
sub DESTROY 
{
    my $self = shift;

    if ($self->xml) {
	print qq(</test>\n);
    } else {
	print "${color_magenta}";
	print '-' x $wchar;
	print "${color_reset}\n";

	print "Tests Passed: [ ${color_green}$self->{pass}${color_reset} ] ";
	print "Tests Failed: [ ${color_red}$self->{fail}${color_reset} ] ";
	print "Total Tests: [ ${color_blue}$self->{tests}${color_reset} ]\n";

	print "${color_magenta}";
	print '-' x $wchar;
	print "${color_reset}\n";

	if ($self->{'die_on_end'} and $self->{'fail'}) {
	    exit 1;
	}
    }
}
###########################################################################

1;
