#!/usr/bin/sh
cd ..
rm -rv build
mkdir build
cd build
cmake -S .. -B .
make
cmake --install . --prefix ~/.local
cd .. && sudo cmake --install build --prefix /usr/local