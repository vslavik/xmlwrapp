#! /usr/bin/perl -wl

use strict;

my $prefix = shift or die "Where is boost located?\n";
my $boost = "$prefix/boost";
my $target = 'include/xmlwrapp';

die "Must be run in the xmlwrapp root directory!\n" unless -f 'configure.pl';
die "Remove include/boost first (rm -rf $target/boost or cvs remove -Rf $target/boost)!\n" if -d "$target/boost";

die "$boost does not exist!\n" unless -d "$boost";

my $files = `grep -r '^#.*include.*boost' include/ src/ | grep -v /boost | awk -F'"' '{print\$2}'`;
$files =~ s/\s+/ /g;
open LIST, "cd $prefix; (find boost/config -type f; for file in $files; do g++ -I. -xc++ -o - -M \$file; done) |";

my %files;

foreach (grep 'boost', <LIST>) {
  chomp;
  foreach (m[boost/\S+]g) { $files{$_} = 1; }
}

close LIST;

$files = join ' ', sort keys %files;
system("(cd $prefix; tar cf - $files) | (cd $target; tar xf -)");
