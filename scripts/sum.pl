#!/usr/bin/perl
use strict;
use warnings;

my @cut = ("nEvents_", "nEvJets_", "nEvLargePtJets_", "nEvMET_", "nEvDeltaPhi_", "nEvBJets_", "nEvGoodBJets_", "nEv0Lepton_", "nEv1Lepton_", "nEv2SSLepton_", "nEv2OSLepton_");
my @cutSum = (0,0,0,0,0,0,0,0,0,0,0);


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
		if ($line =~ /($_)( = )(\S*)/){
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
	print "$_ = $cutSum[$element] ($percent%)\n";
    }
    else{
	print "$_ = $cutSum[$element]\n";
    }
    $lastValue = $cutSum[$element];
    $element += 1;
}
