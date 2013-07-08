#!/usr/bin/perl -w

push @INC, "/mnt/data/TestCode/perl/NML/lib/";

use lib "/mnt/data/TestCode/perl/NML/lib/";

use NML;

my $buf = '<?xml version="1.0" encoding="UTF-8"?> <Volume_Collection> <Volume resource-id="data" resource-type="Volume"/> </Volume_Collection>';
my $volumes = "";
&NML::getVolumes($buf, $volumes);
print "$volumes\n";
