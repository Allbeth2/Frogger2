#!/bin/bash

export CXX=/var/lib/flatpak/runtime/org.freedesktop.Sdk/x86_64/25.08/13c148466d130b011ca9f8c52708abcbf6cc06f2244fb899d5b03daeb4d0531e/files/bin/g++
export LD_LIBRARY_PATH=/var/lib/flatpak/runtime/org.freedesktop.Sdk/x86_64/25.08/13c148466d130b011ca9f8c52708abcbf6cc06f2244fb899d5b03daeb4d0531e/files/lib/x86_64-linux-gnu

rm -rf build
mkdir -p build
cd build
/var/lib/flatpak/runtime/org.freedesktop.Sdk/x86_64/25.08/13c148466d130b011ca9f8c52708abcbf6cc06f2244fb899d5b03daeb4d0531e/files/bin/cmake ..
make

cd build

./Frogger2
