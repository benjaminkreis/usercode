#!/usr/bin/perl
use strict;
use warnings;

my @cut = ("nEvents:", "nPass0:","nPass1:","nPass2:","nPass3:","nPass4:","nPass5:","nPass6:","nPass7:","nPass8a:","nPass8b:","nPass8c:");
my @cutSum = (0,0,0,0,0,0,0,0,0,0,0,0);


opendir (DIR, '.') or die "Couldn't open directory, $!";
while ( my $file = readdir DIR )
{
    if( $file =~ /stdout/ ){
    #print "open $file\n";
   
	open(my $fin, "<", "$file") or die "Can't open $file: $!";
	
	while (<$fin>){
	    my $line = $_;
	    
	    my $element = 0;
	    foreach(@cut){
		if ($line =~ /($_)( )(\S*)/){
		    $cutSum[$element] += $3;
		}
		$element += 1;
	    }
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
