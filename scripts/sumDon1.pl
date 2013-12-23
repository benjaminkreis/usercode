#!/usr/bin/perl
use strict;
use warnings;

my @cut = ("nEvents:", "nPass0_:","nPass1_:","nPass2_:","nPass3_:","nPass4_:","nPass5_:","nPass6_:","nPass7_:","nPass7p5_:","nPass8a_:","nPass8b_:","nPass8c_:");
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
		if ($line =~ /($_)( )(\S*)( )(\S*)/){
		    if ($cutCheck[$element]!=0) {print "MORE THAN ONE\n";}
		    $cutSum[$element] += $3;
		    $cutCheck[$element] += 1;
		}
		$element += 1;
	    }
	}
	foreach(@cutCheck){
	    if($_ != 1){ print "INCOMPLETE \n"; }
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
#	print "$_ $cutSum[$element]\n";
	print "$cutSum[$element]\n";

    }
    else{
#	print "$_ $cutSum[$element]\n";
	print "$cutSum[$element]\n";
    }
    $lastValue = $cutSum[$element];
    $element += 1;
}
