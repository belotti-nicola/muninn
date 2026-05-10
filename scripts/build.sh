#!/usr/bin/sh
cd ..
rm -rv build
mkdir build
cd build
cmake -S .. -B .
make
cmake --install . --prefix ~/.local