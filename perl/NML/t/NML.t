# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl NML.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use strict;
use warnings;

use Test::More tests => 9;
BEGIN { use_ok('NML') };

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

# Test2: test with one volume.
my $buf = '<?xml version="1.0" encoding="UTF-8"?> <Volume_Collection> <Volume resource-id="data" resource-type="Volume"/> </Volume_Collection>';
my $volumes = "";
my $ret = &NML::getVolumes($buf, $volumes);
ok($ret == 0 && $volumes eq "data", "testSingleVolume");

# Test3: test with two volumes.
$buf = '<?xml version="1.0" encoding="UTF-8"?> <Volume_Collection> <Volume resource-id="shared" resource-type="Volume"/><Volume resource-id="data" resource-type="Volume"/> </Volume_Collection>';
$volumes = "";
$ret = &NML::getVolumes($buf, $volumes);
ok($ret == 0 && $volumes eq "shared,data", "testTwoVolume");

# Test4: test with wrong XML.
$buf = 'lume_Collection> <Volume resource-id="shared" resource-type="Volume"/><Volume resource-id="data" resource-type="Volume"/> </Volume_Collection>';
$volumes = "";
$ret = &NML::getVolumes($buf, $volumes);
ok($ret == -2, "testWrongXML");

# Test5: test with empty XML.
$buf = '';
$volumes = "";
$ret = &NML::getVolumes($buf, $volumes);
ok($ret == -1, "testEmptyXML");

# Test6: test with normal volume detail.
$buf = <<EOF;
<?xml version="1.0" encoding="UTF-8"?>
<Volume_Collection>
  <Volume resource-id="data" resource-type="Volume" ejectable="false">
    <Property_List>
      <Volume_Name>data</Volume_Name>
      <Volume_Mode>1</Volume_Mode>
      <AltRoot/>
      <DeDup/>
      <AutoExpand>on</AutoExpand>
      <AutoReplace>on</AutoReplace>
      <CacheFile/>
      <Delegation>on</Delegation>
      <ListSnapshots>on</ListSnapshots>
      <Version>0</Version>
      <Allocated>2923321800</Allocated>
      <Capacity>2923321800</Capacity>
      <Free>2923116828</Free>
      <Available>2923116828</Available>
      <GUID>98ff9569-fbd7-44ba-a28c-bfc5e4c45f18</GUID>
      <Health>UNPROTECTED</Health>
      <Checksum>on</Checksum>
      <Encryption enabled="0"/>
    </Property_List>
    <vdev_list>
      <RAID LEVEL="1" ID="0">
        <Disk resource-id="sdb"/>
      </RAID>
    </vdev_list>
  </Volume>
</Volume_Collection>
EOF
my $total = "";
my $free = "";
my $volume = "data";
$ret = &NML::getVolumeDetail($buf, $volume, $total, $free);
ok($ret == 0 && $total eq "2923321800" && $free eq "2923116828", "testVolumeDetailNormal");

# Test7: test with empty XML.
$buf = '';
$ret = &NML::getVolumeDetail($buf, $volume, $total, $free);
ok($ret == -1, "testEmptyXML");

# Test8: test with wrong XML.
$buf = 'lume_Collection> <Volume resource-id="shared" resource-type="Volume"/><Volume resource-id="data" resource-type="Volume"/> </Volume_Collection>';
$ret = &NML::getVolumeDetail($buf, $volume, $total, $free);
ok($ret == -2, "testWrongXML");

# Test9: test with no volume detail.
$buf = <<EOF;
<?xml version="1.0" encoding="UTF-8"?>
<Volume_Collection>
  <Volume resource-id="data" resource-type="Volume" ejectable="false">
    <Property_List>
      <Volume_Name>data</Volume_Name>
      <Volume_Mode>1</Volume_Mode>
      <AltRoot/>
      <DeDup/>
      <AutoExpand>on</AutoExpand>
      <AutoReplace>on</AutoReplace>
      <CacheFile/>
      <Delegation>on</Delegation>
      <ListSnapshots>on</ListSnapshots>
      <Version>0</Version>
      <Allocated>2923321800</Allocated>
      <Capacity>2923321800</Capacity>
      <Free>2923116828</Free>
      <Available>2923116828</Available>
      <GUID>98ff9569-fbd7-44ba-a28c-bfc5e4c45f18</GUID>
      <Health>UNPROTECTED</Health>
      <Checksum>on</Checksum>
      <Encryption enabled="0"/>
    </Property_List>
    <vdev_list>
      <RAID LEVEL="1" ID="0">
        <Disk resource-id="sdb"/>
      </RAID>
    </vdev_list>
  </Volume>
</Volume_Collection>
EOF
$total = "";
$free = "";
$volume = "nonexisted";
$ret = &NML::getVolumeDetail($buf, $volume, $total, $free);
ok($ret == 0 && $total eq "" && $free eq "", "testVolumeDetailNon");

