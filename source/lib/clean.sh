#!/bin/bash

export LC_ALL=en_US.UTF-8

cd `dirname "$(realpath $0)"`

rm -f *.o

if [ ! -f /usr/bin/astyle ]; then exit 0; fi

f() {
 /usr/bin/astyle \
  --style=linux \
  --indent=spaces=2 \
  --suffix=none \
  --pad-header \
  --unpad-paren \
  --quiet \
  "$1"
}

for m in *.c; do
  f "$m"
done;

