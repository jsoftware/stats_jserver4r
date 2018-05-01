#/bin/bash
#
# make used by author (not distributed)

if [ "Darwin" == `uname` ]; then
  ext=dylib
  export LIBRARY_PATH=/usr/local/lib:$LIBRARY_PATH
else
  ext=so
fi

cd ~/deb/public/stats/r/jserver4r/lib
make
cp -p libjserver4r* ~/j8/addons/stats/jserver4r/lib

