#!/usr/bin/sh
cd ..
rm -rv build
mkdir build
cd build
cmake ..
make