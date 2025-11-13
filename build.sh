1#!/bin/bash
set -e
rm -rf build
mkdir -p build
cd build
cmake ..
make
./Frogger2