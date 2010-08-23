#!/usr/bin/perl
use strict;
use warnings;

my $name = "vqq_7tev";
my $min = 1;
my $max = 41;

open(my $out, ">", "list.txt") or die "Can't open output file for $name: $!";

for (my $i = $min; $i <= $max; $i++){
    print $out "\"rfio:/castor/cern.ch/user/k/kreis/vqq/$name\_$i.root\",\n"
#   print $out "rm /castor/cern.ch/user/k/kreis/$name\_$i.root\n"
}

print "list complete\n"
