#!/bin/sh

export CC=gcc
export LD=ld
export AS=as
export AR=ar
export CFLAGS=
export LDFLAGS=

cd tools/kconfig-frontends/

echo configure mconf tool...wait a few seconds...
./configure \
  --enable-mconf \
  --disable-utils \
  --disable-L10n \
  --disable-conf \
  --disable-nconf \
  --disable-gconf \
  --disable-qconf \
  2>&1 > /dev/null || exit "$$?";

make || exit "$$?";
