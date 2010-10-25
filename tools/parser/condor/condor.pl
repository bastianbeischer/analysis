#!/usr/bin/perl -w

use strict;
#use File::Find;

my $exe = "$ENV{PERDAIXANA_PATH}/bin/parser";
my $pwd = "$ENV{PWD}";
my $condorDir = "$ENV{PERDAIXANA_PATH}/tools/parser/condor";

my @files = sort(@ARGV);

unless (-d "$condorDir") {
    mkdir "$condorDir";
}
unless (-d "$condorDir/condor") {
    mkdir "$condorDir/condor";
}
unless (-d "$condorDir/ERR") {
    mkdir "$condorDir/ERR";
}
unless (-d "$condorDir/LOG") {
    mkdir "$condorDir/LOG";
}
unless (-d "$condorDir/STD") {
    mkdir "$condorDir/STD";
}

unless (-e "$exe") {
    print "ERROR: executable does not exist. Check script!"
}

my $nJobs = scalar @files;
for (my $argnum = 0; $argnum < $nJobs; $argnum += 1) {
 
    my $arguments = "$files[$argnum]";
    my $condorfile = &make_condor_file($arguments);

    print "Submitting job number $argnum with arguments: $arguments\n";
    system "condor_submit", "$condorfile"; 
}

# Make condor directory (and all files/dirs within) writable for group.
#find(sub {chmod 0770, $_}, ${condorDir});

print "\n";
print "-----------------------------------------------------------\n";
print "Submitted $nJobs jobs in total.\n";
print "\n";

sub make_condor_file {

    my $arguments = shift;

    my $begin = rindex($arguments, "/")+1;
    my $end   = rindex($arguments, "\.") - $begin;
    
    my $condorFileStem = substr($arguments, $begin, $end);

    open CONDORFILE, ">${condorDir}/condor/${condorFileStem}.condor" or die "Error: Cannot make condor file";

print CONDORFILE <<EOF;
universe        = vanilla
executable      = ${exe}
arguments	= ${arguments}
initialdir      = ${pwd}
EOF

print CONDORFILE <<'EOF';
getenv          = true
requirements    = ( OpSys == "LINUX" )
rank            = KFlops
EOF

print CONDORFILE <<EOF;
error           = ${condorDir}/ERR/${condorFileStem}.ERR
output          = ${condorDir}/STD/${condorFileStem}.STD
log             = ${condorDir}/LOG/${condorFileStem}.LOG
EOF

print CONDORFILE <<'EOF';
notification    = error
notify_user	= beischer@physik.rwth-aachen.de
coresize        = 1000
Queue
EOF

    close CONDORFILE;

    # return value: condor file name
    "${condorDir}/condor/${condorFileStem}.condor";

}
