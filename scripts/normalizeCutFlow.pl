#!/usr/bin/perl
use strict;
use warnings;

my $weight = 39.15117868;

open(my $fin,  "<",  "cutflow.txt")  or die "Can't open cut flow : $!";
    
while (<$fin>){     # assigns each line in turn to $_  
    if(/(\S*)(\s)(=)(\s)(\S*)(\s)(\S*)/){
	my $value = $5;
	my $valueError = sqrt($5);
	my $normalizedValue = $value*$weight;
	my $normalizedValueError = $valueError*$weight; 
	print "$1 = $value, $normalizedValue +- $normalizedValueError\n";
	
    }
}
