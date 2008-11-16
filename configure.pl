#! /usr/bin/perl
######################################################################
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
################################################################################
#
# configure.pl (bootstrap xmlwrapp)
# Peter J Jones (pjones@pmade.org)
#
################################################################################
#
# Includes
#
################################################################################
use strict;
use Getopt::Long;
use Cwd qw(cwd chdir);
################################################################################
#
# Constants
#
################################################################################
use constant DATE		=> 'Tue Jan 15 08:56:06 2002';
use constant ID			=> '$Id: configure.pl,v 1.6 2005-11-15 21:30:43 tbrowder2 Exp $';
################################################################################
#
# Global Variables
#
################################################################################
$Getopt::Long::bundling = 1;

my $cwd = cwd();
my %clo;
my $dirsep = "/";

my $mkmf	= "${cwd}${dirsep}tools${dirsep}mkmf";
my $cxxflags	= "${cwd}${dirsep}tools${dirsep}cxxflags";
my $genconfig	= "${cwd}${dirsep}tools${dirsep}genconfig";
my $master_inc	= "${cwd}${dirsep}include";

my $mkmf_flags;
my $libname	= "xmlwrapp";
my $install_spec= "docs${dirsep}install.spec";
my $src_sub_dir = 'libxml';
my @test_dirs = qw(tree event node document attributes);
my $xmlwrapp_ver;
my $xmlwrapp_mjr;

my $xslt_libname    = "xsltwrapp";
my $xslt_sub_dir    = 'libxslt';
my $xsltwrapp_okay  = 0;
my $xsltwrapp_mjr;

my $have_pkg_config = 0;
my @external_libs;
my @external_incs;
################################################################################
#
# Code Start
#
################################################################################
$|++;

if (not defined $ENV{'CXX'}) {
    print STDERR "**** Your CXX environment variable is not set.               ****\n";
    print STDERR "**** xmlwrapp needs this variable to find your C++ compiler. ****\n";
    exit;
}

GetOptions(
    \%clo,

    'help|h',

    'developer|d',
    'contrib',

    'xml2-config=s',
    'xslt-config=s',

    'disable-shared',
    'disable-examples',
    'enable-tests|t',
    'disable-xslt',

    'prefix=s',
    'bindir=s',
    'libdir=s',
    'incdir=s',
) or usage();
$clo{'help'} && usage();

sub usage {
    print "Usage: $0 [options]\n", <<EOT;
  -h, --help           This message
  --developer          Turn on features for a xmlwrapp developer
  --contrib            Configure for being bundled inside another project
  --disable-shared     Don't build a shared library
  --disable-examples   Don't build the example programs
  --enable-tests       Enable the building of test programs

  --xml2-config file   Run file to get info about libxml2 [xml2-config]

  --prefix path        Set install prefix to path [/usr/local]
  --bindir path        Set bin install location to path [PREFIX/bin]
  --libdir path        Set lib install location to path [PREFIX/lib]
  --incdir path        Set include install location to path [PREFIX/include]
EOT
    exit 1;
}

# set some defaults
$clo{'prefix'}	||= "/usr/local";
$clo{'bindir'}	||= "$clo{'prefix'}${dirsep}bin";
$clo{'libdir'}	||= "$clo{'prefix'}${dirsep}lib";
$clo{'incdir'}	||= "$clo{'prefix'}${dirsep}include";
$clo{'disable-shared'} = 1 if $clo{'contrib'};

if ($clo{'contrib'}) {
    $clo{'disable-shared'} = 1;
    $clo{'disable-examples'} = 1;
}

# setup defaults for mkmf
$mkmf_flags = "--include $master_inc --cxxflags $cxxflags --quiet ";
if ($clo{'developer'}) {$mkmf_flags .= " --developer";}

# look for libxml2
if ($clo{'disable-xslt'} or check_libxslt() != 1) {
    check_libxml2();
}

# find out the current version
parse_version_file();

# generate the config script
generate_config_script();

# generate the xmlwrapp_config.h header
generate_config_header();

# start generating makefiles
generate_top_makefile();
generate_test_makefiles();
generate_example_makefiles();
generate_src_makefiles();

if (!$clo{'contrib'}) {
    print "+---------------------------------------------------------------+\n";
    print "|      Join the users mailing list for help with xmlwrapp.      |\n";
    print "|       Visit 'http://sourceforge.net/projects/xmlwrapp'.       |\n";
    print "|                      Click on 'Lists'.                        |\n";
    print "+---------------------------------------------------------------+\n";
}
################################################################################
sub parse_version_file {
    unless (open(VERSION, "docs${dirsep}VERSION")) {
	print STDERR "$0: can't open VERSION file 'docs${dirsep}VERSION': $!\n";
	exit 1;
    }

    my $first_line = <VERSION>;
    close VERSION;

    ($xmlwrapp_ver, $xmlwrapp_mjr, $xsltwrapp_mjr, undef) = split(/\s+/, $first_line, 4);
}
################################################################################
sub check_libxslt {
    my $output;

    print "Checking for libxslt version ... ";
    $clo{'xslt-config'} ||= 'xslt-config';
    chomp($output = `$clo{'xslt-config'} --version 2>&1`);

    if ($? != 0 or not defined $output or not length($output)) {
	print "no\n";
	return 0;
    }

    if ($output =~ /^(\d+\.\d+)\.(\d+)$/) {
	if (($1 > 1.0) or ($1 == 1.0 and $2 >= 23)) {
	    print "$output >= 1.0.23\n";
	} else {
	    print "$output < 1.0.23\n";
	    print STDERR "**** your version of libxslt is too old.\n";
	    print STDERR "**** you can configure with --disable-xslt to drop XSLT support.\n";
	    exit 1;
	}
    } else {
	print "xmlwrapp bug\n";
	print STDERR "**** I can't parse the version string, this is a bug!\n";
	exit 1;
    }

    load_flags_from_xml_config("xslt");
    $xsltwrapp_okay = 1;
    push(@test_dirs, "xslt");

    return 1;
}
################################################################################
sub check_libxml2 {
    my $output;

    print "Checking for libxml2 version ... ";
    $clo{'xml2-config'} ||= 'xml2-config';
    chomp($output = `$clo{'xml2-config'} --version 2>&1`);

    unless ($output) {
	print "[fail]\n";
	print STDERR "**** can't find xml2-config, try using --xml2-config\n";
	exit 1;
    }

    if ($output =~ /^(\d+\.\d+)\.(\d+)$/) {
	if (($1 >= 2.5) || $1 == 2.4 && $2 >= 28) {
	    print "$output >= 2.4.28\n";
	} else {
	    print "$output < 2.4.28\n";
	    print STDERR "**** your version of libxml2 is too old, please upgrade\n";
	    exit 1;
	}
    } else {
	print "what is that?\n";
	print STDERR "**** I can't seem to parse the libxml2 version, please submit a bug\n";
	exit 1;
    }

    load_flags_from_xml_config("xml2");
}
################################################################################
sub load_flags_from_xml_config {
    my $xmllib = shift;
    my $config = "$xmllib-config";
    my $output;

    print "Finding XML '$xmllib' include directory ... ";
    chomp($output = `$clo{$config} --cflags 2>&1`);

    if ($xmllib eq 'xslt' && 1) {
      # hack
      $output = $output . ' -I/usr/local/include/libxml2';
      if (0) {
	my $cmd = $clo{$config};
	print "DEBUG: cmd = '$cmd'...\n";
	print "DEBUG: xml2 flags = '$output'...\n";
	exit;
      }
    }
    if ($? != 0 or not defined $output or not length($output)) {
	print "fail\n";
	print "**** error running $clo{$config} --cflags, sorry\n";
	exit 1;
    }

    $output =~ s/-I//g;
    my @include_dirs = split(/\s+/, $output);
    my $main_include_dir = undef;
    my $xslt_include_dir = undef;

    # add a few standard directories just in case
    push(@include_dirs, '/usr/include');

    foreach my $dir (@include_dirs) {
	if (-d "$dir${dirsep}libxml" and not defined $main_include_dir) {
	    $main_include_dir = $dir;
	}

	if (-e "$dir${dirsep}libxslt${dirsep}transform.h" and not defined $xslt_include_dir) {
	    $xslt_include_dir = $dir;
	}

	if (-e "$dir${dirsep}giconv.h") {
	    push(@external_incs, $dir);
	}

	if (-e "$dir${dirsep}iconv.h") {
	    push(@external_incs, $dir);
	}
    }

    if ($xmllib eq 'xslt' and not defined $xslt_include_dir) {
      print STDERR "**** can't find include dir for libxslt, what does $clo{'$xmllib-config'} --cflags say?\n";
      exit 1;
    }
    elsif($xmllib eq 'xslt') {
      push(@external_incs, $xslt_include_dir);
    }

    if (not defined $main_include_dir) {
      print "fail\n";
      print STDERR "**** can't find include dir for libxml2, what does $clo{'$xmllib-config'} --cflags say?\n";
      exit 1;
    }
    else {
      print "$main_include_dir\n";
      push(@external_incs, $main_include_dir);
    }

    print "Finding XML libraries ... ";
    chomp($output = `$clo{$config} --libs 2>&1`);

    if ($? != 0 or not defined $output or not length($output)) {
      print "fail\n";
      print "**** error running $clo{$config} --libs, sorry\n";
      exit 1;
    }
    else {
      print "done\n";
    }

    $output =~ s/^\s+//; $output =~ s/\s+$//;
    my $last_L;

    if ($xmllib eq 'xslt' and $output !~ /-lexslt/) {
	$output =~ s/-lxslt/-lxslt -lexslt/;
    }

    while ($output =~ /-([Ll])(\S+)/cg) {
	if ($1 eq 'L') {
	    $last_L = $2;
	    next;
	}

	if ($last_L) {
	    push(@external_libs, [$last_L, $2]);
	} else {
	    push(@external_libs, $2);
	}
    }
}
################################################################################
sub generate_top_makefile {
    my $extra_dirs = "tests" if $clo{'enable-tests'};
    $extra_dirs .= " examples" if not $clo{'disable-examples'};

    my $extra_flags = '';
    $extra_flags .= ' --with-test tests' if $clo{'enable-tests'};

    unless (open(SPEC, ">$install_spec")) {
	print STDERR "\n$0: can't open $install_spec: $!\n";
	exit 1;
    }

    # set some install paths
    print SPEC "bindir=$clo{'bindir'}\n";
    print SPEC "libdir=$clo{'libdir'}\n";
    print SPEC "includedir=$clo{'incdir'}\n";

    # add files
    print SPEC "binary $libname-config\n";
    print SPEC "pkgconfig $libname.pc\n" if $have_pkg_config == 1;
    print SPEC "include-dir include/$libname $libname\n";
    print SPEC "static-lib src/$src_sub_dir $libname\n";
    print SPEC "shared-lib src/$src_sub_dir $libname $xmlwrapp_mjr\n" unless $clo{'disable-shared'};

    if ($xsltwrapp_okay == 1) {
	print SPEC "include-dir include/$xslt_libname $xslt_libname\n";
	print SPEC "static-lib src/$xslt_sub_dir $xslt_libname\n";
	print SPEC "shared-lib src/$xslt_sub_dir $xslt_libname $xsltwrapp_mjr\n" unless $clo{'disable-shared'};
    }

    close SPEC;

    print "Creating Makefile ...\n";
    system("$^X $mkmf $mkmf_flags --install $install_spec $extra_flags --wrapper src $extra_dirs");
    unlink($install_spec);
}
################################################################################
sub generate_src_makefiles {
    ##
    # src/Makefile
    if (! chdir("$cwd${dirsep}src")) {
	print STDERR "\n**** hey, I can't cd into my src directory: $!\n";
	exit 1;
    }

    my $src_dirs = "$src_sub_dir";
    $src_dirs .= " $xslt_sub_dir" if $xsltwrapp_okay == 1;

    print "Creating src/Makefile ...\n";
    system("$^X $mkmf $mkmf_flags --wrapper $src_dirs");
    chdir($cwd);

    ##
    # src/libxml2/Makefile
    if (! chdir("$cwd${dirsep}src${dirsep}$src_sub_dir")) {
	print STDERR "\n**** hey, I can't cd into my src/$src_sub_dir directory: $!\n";
	exit 1;
    }

    my $extra_flags = "--shared-lib $libname --major $xmlwrapp_mjr" unless $clo{'disable-shared'};
    foreach my $dir (@external_incs) { $extra_flags .= " --include $dir"; }

    print "Creating src/$src_sub_dir/Makefile ...\n";
    system("$^X $mkmf $mkmf_flags --static-lib $libname $extra_flags *.cxx");
    chdir($cwd);

    ##
    # src/libxslt/Makefile
    if (! chdir("$cwd${dirsep}src${dirsep}$xslt_sub_dir")) {
	print STDERR "\n**** hey, I can't cd into my src/$xslt_sub_dir directory: $!\n";
	exit 1;
    }

    my $extra_flags = "--shared-lib $xslt_libname --major $xsltwrapp_mjr" unless $clo{'disable-shared'};
    foreach my $dir (@external_incs) { $extra_flags .= " --include $dir"; }

    print "Creating src/$xslt_sub_dir/Makefile ...\n";
    system("$^X $mkmf $mkmf_flags --static-lib $xslt_libname $extra_flags *.cxx");
    chdir($cwd);
}
################################################################################
sub generate_test_makefiles {
    return unless $clo{'enable-tests'};

    if (! chdir("$cwd${dirsep}tests")) {
	print STDERR "\n$0: can't cd to tests: $!\n";
	exit 1;
    }

    my $with_test = join(',', @test_dirs);
    print "Creating tests/Makefile ...\n";
    system("$^X $mkmf $mkmf_flags --with-test '$with_test' --wrapper @test_dirs");
    chdir($cwd);

    # static link with xmlwrapp so that it does not need to be installed
    my $extra_flags = "";
    $extra_flags .= "--slinkwith $cwd${dirsep}src/$xslt_sub_dir,$xslt_libname " if $xsltwrapp_okay == 1;
    $extra_flags .= "--slinkwith $cwd${dirsep}src/$src_sub_dir,$libname ";

    # link with all external libs
    foreach my $lib (@external_libs) {
	if (ref $lib) {
	    $extra_flags .= " --linkwith $lib->[0],$lib->[1]";
	} else {
	    $extra_flags .= " --linkwith $lib";
	}
    }

    $extra_flags .= " --test-cmds '$^X runtest.pl'";

    foreach my $test (@test_dirs) {
	if (! chdir("$cwd${dirsep}tests${dirsep}$test")) {
	    print STDERR "\n$0: can't cd to $cwd${dirsep}tests${dirsep}$test: $!\n";
	    exit 1;
	}

	print "Creating tests/$test/Makefile ...\n";
	system("$^X $mkmf $mkmf_flags --many-exec $extra_flags *.cxx");
	chdir($cwd);
    }
}
################################################################################
sub generate_example_makefiles {
    return if $clo{'disable-examples'};

    if (! chdir("$cwd${dirsep}examples")) {
	print STDERR "\n$0: can't cd to examples: $!\n";
	exit 1;
    }

    my @raw_example_dirs = grep {!/^(CVS|Makefile)$/} glob("*");
    my @example_dirs;

    foreach (@raw_example_dirs) {
	if (/xslt/i and $xsltwrapp_okay != 1) { next; }
	push(@example_dirs, $_);
    }

    print "Creating examples/Makefile ...\n";
    system("$^X $mkmf $mkmf_flags --wrapper @example_dirs");
    chdir($cwd);

    # static link with xmlwrapp so that it does not need to be installed
    my $extra_flags;
    $extra_flags .= "--slinkwith $cwd${dirsep}src/$xslt_sub_dir,$xslt_libname " if $xsltwrapp_okay == 1;
    $extra_flags .= "--slinkwith $cwd${dirsep}src${dirsep}$src_sub_dir,$libname ";

    # link with all external libs
    foreach my $lib (@external_libs) {
	if (ref $lib) {
	    $extra_flags .= " --linkwith $lib->[0],$lib->[1]";
	} else {
	    $extra_flags .= " --linkwith $lib";
	}
    }

    foreach my $example (@example_dirs) {
	if (! chdir("$cwd${dirsep}examples${dirsep}$example")) {
	    print STDERR "\n$0: can't cd to $cwd${dirsep}examples${dirsep}$example: $!\n";
	    exit 1;
	}

	print "Creating examples/$example/Makefile ...\n";
	system("$^X $mkmf $mkmf_flags --one-exec example $extra_flags *.cxx");
	chdir($cwd);
    }
}
################################################################################
sub generate_config_script {
    my ($all_incs, $all_libs);

    if ($clo{'contrib'}) {
	$all_incs = "-I$cwd${dirsep}include ";
	$all_libs = "";

	my @lib_names;
	push(@lib_names, $xslt_libname) if $xsltwrapp_okay == 1;
	push(@lib_names, $libname);

	foreach my $lib_name (@lib_names) {
	    my $full_libname = `$^X $cxxflags --static-lib-prefix`;
	    $full_libname =~ s/^\s+//; $full_libname =~ s/\s+$//;
	    $full_libname .= $lib_name;
	    $full_libname .= `$^X $cxxflags --static-lib-extension`;
	    $full_libname =~ s/^\s+//; $full_libname =~ s/\s+$//;

	    my $subdir = $lib_name eq $xslt_libname ? $xslt_sub_dir : $src_sub_dir;
	    $all_libs .= "$cwd${dirsep}src${dirsep}$subdir${dirsep}$full_libname ";
	}
    } else {
	$all_incs  = "-I$clo{'incdir'} ";
	$all_libs .= `$^X $cxxflags --linkwith $clo{'libdir'},$xslt_libname` if $xsltwrapp_okay == 1;
	$all_libs .= `$^X $cxxflags --linkwith $clo{'libdir'},$libname`;
    }

    # add external libraries
    foreach my $lib (@external_libs) {
	if (ref $lib) {
	    $all_libs .= `$^X $cxxflags --linkwith $lib->[0],$lib->[1]`;
	} else {
	    $all_libs .= `$^X $cxxflags --linkwith $lib`;
	}
    }

    # clean and encode
    foreach ($all_incs, $all_libs) {
	s/^\s+//; s/\s+$//; s/\s+/ /g; s/-/^/g;
    }

    my $command  = "$^X $genconfig --version $xmlwrapp_ver --name $libname";
       $command .= " --libs \"$all_libs\" --cxxflags \"$all_incs\"";
       $command .= " --desc 'A C++ wrapper around libxml2 and libxslt' ";
       $command .= " -o $libname-config";

    # check for pkg-config
    if (system("pkg-config --version > /dev/null 2>&1") == 0 || system("$clo{'bindir'}/pkg-config --version > /dev/null 2>&1") == 0) {
	$have_pkg_config = 1;
	$command .= " --prefix='$clo{prefix}' --bindir='$clo{bindir}' --libdir='$clo{libdir}' --incdir='$clo{incdir}' --pkgconfig='$libname.pc'";
    }

    print "Creating xmlwrapp-config script ... \n";
    system("$command");
}
################################################################################
sub generate_config_header {
    if (not open(CH, ">src${dirsep}xmlwrapp_config.h")) {
	print STDERR "$0: error creating src/xmlwrapp_config.h: $!\n";
	exit 1;
    }

    print CH "#ifndef _xmlwrapp_config_h_\n";
    print CH "#define _xmlwrapp_config_h_\n";

    if ($xsltwrapp_okay == 1) {
	print CH "#define XMLWRAPP_WITH_XSLT\n";
    }

    print CH "#endif\n";
    close CH;
}
################################################################################
