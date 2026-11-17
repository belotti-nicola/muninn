#!/usr/bin/sh

cd ..
rm -rv build
mkdir build
cd build

cmake --install . --prefix /home/nicola/Projects/muninn/install -DCMAKE_BUILD_TYPE=Debug

make