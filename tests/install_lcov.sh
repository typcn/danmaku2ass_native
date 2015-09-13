#!/bin/sh
set -e
if [ ! -d "$HOME/protobuf/lib" ]; then
  wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.11.orig.tar.gz;
  tar xf lcov_1.11.orig.tar.gz;
  export PREFIX=$HOME/lcov;
  make -C lcov-1.11/ install;
else
  echo 'Using cached lcov.';
fi