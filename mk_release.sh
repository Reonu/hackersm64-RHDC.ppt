export THE_VERSION="v1.0.1"
export THE_VERSION_DIR="build/release/${THE_VERSION}"
make patch
mkdir $THE_VERSION_DIR
cp build/us_n64/yesimmakingacomphack.z64 $THE_VERSION_DIR/RHDC.ppt.$THE_VERSION.z64
cp build/us_n64/yesimmakingacomphack.bps $THE_VERSION_DIR/RHDC.ppt.$THE_VERSION.bps
cp build/us_n64/sm64.us.map $THE_VERSION_DIR/RHDC.ppt.$THE_VERSION.map
