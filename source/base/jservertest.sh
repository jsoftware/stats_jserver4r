#!/bin/bash
#
# example of calling R script with lib path set

if [ "Darwin" == `uname` ]; then
  export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH
fi

cd ~/R
Rscript "jservertest.R"
