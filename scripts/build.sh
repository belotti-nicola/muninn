#!/usr/bin/sh

cd ..
rm -rv build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug && make 
