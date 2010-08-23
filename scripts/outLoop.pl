#!/usr/bin/perl
use strict;
use warnings;

open(my $fin, "<", "mg2list.dat") or die "Can't open input: $1";

my @filesArray = <$fin>;

print "begin\n";

foreach(@filesArray){
    
    my $thisFile = $_;
    #print "File: $thisFile";
    
    if($thisFile =~ /(\S*)(_)(\d*)(_)(\d*)(_)(\S*)(root)/){
	my $thisFileNum = $3;
	my $duplicate = 0;
	foreach(@filesArray){
	    
	    my $thisFile2 = $_;
	    if($thisFile2 =~ /(\S*)(_)(\d*)(_)(\d*)(_)(\S*)(root)/){
		
		if($3 == $thisFileNum){
		    $duplicate+=1;
		}#match 
		    
		}#thisFile2
	    }#foreach2
	    
	    
	    if($duplicate>=2){
		print "$thisFile";
	    }
	
	
    }#thisFile
}#foreach
print "done\n";
