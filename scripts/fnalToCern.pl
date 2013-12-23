#!/usr/bin/perl
use strict;
use warnings;

my $min = 1;
my $max = 54;

open(my $out, ">", "copy_zmumu.sh") or die "Can't open output file: $!";

for (my $i = $min; $i <= $max; $i++){
    print $out "lcg-cp --verbose -b -D srmv2 \"srm://cmssrm.fnal.gov:8443/srm/managerv2?SFN=/resilient/wdteo/zmumu/wteo/Zmumu/wteo_zmumu7tev_fat/d7e560b5d051c08e1d60b735763a6025/zmumu_7tev_$i.root\" \"srm://srm-cms.cern.ch:8443//srm/managerv2?SFN=/castor/cern.ch/user/k/kreis/zmumu/zmumu_7tev_$i.root\"\n"
}

print "complete\n"
