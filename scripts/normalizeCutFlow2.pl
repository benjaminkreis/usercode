#!/usr/bin/perl
use strict;
use warnings;

my $weight = 0.002260798;

open(my $fin,  "<",  "cutflow.txt")  or die "Can't open cut flow : $!";
    
while (<$fin>){     # assigns each line in turn to $_  
    if(/(\S*)(:)(\s)(\S*)(\s)/){
	my $value = $4;
	my $valueError = sqrt($4);
	my $normalizedValue = $value*$weight;
	my $normalizedValueError = $valueError*$weight; 
	print "$1 = $value, $normalizedValue +- $normalizedValueError\n";
	
    }
}
