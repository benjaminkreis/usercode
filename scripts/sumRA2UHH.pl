#!/usr/bin/perl
use strict;
use warnings;

my @cut = ("nEvPass0:", "nEvPass1:", "nEvPass2:", "nEvPass3:", "nEvPass4:", "nEvPass5:", "nEvPass6:", "nEvPass7:", "eEvPass1:", "eEvPass4:", "eEvPass5:", "eEvPass6:", "eEvPass7:");
my @cutSum = (0,0,0,0,0,0,0,0,0,0,0,0,0);


opendir (DIR, '.') or die "Couldn't open directory, $!";
while ( my $file = readdir DIR )
{
    my @cutCheck = (0,0,0,0,0,0,0,0,0,0,0,0,0);
    if( $file =~ /stdout/ ){
    #print "open $file\n";
   
	open(my $fin, "<", "$file") or die "Can't open $file: $!";
	
	while (<$fin>){
	    my $line = $_;
	    
	    my $element = 0;
	    foreach(@cut){
		if ($line =~ /($_)( )(\S*)/){
		    $cutSum[$element] += $3;
		    $cutCheck[$element] +=1;
		}
		$element += 1;
	    }
	}
	foreach(@cutCheck){
	    if($_ != 1){ print "INCOMPLETE \n";}
	}
    }
}
closedir DIR;

my $percent = 0;
my $lastValue = 0;
my $element = 0;
foreach(@cut){
    if($lastValue!=0){
	$percent = $cutSum[$element]/$lastValue*100;
#	print "$_ $cutSum[$element] ($percent%)\n";
	print "$_ $cutSum[$element]\n";
    }
    else{
	print "$_ $cutSum[$element]\n";
    }
    $lastValue = $cutSum[$element];
    $element += 1;
}
