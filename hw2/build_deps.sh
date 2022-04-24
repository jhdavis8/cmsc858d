#!/bin/bash

cd FastaReader
mkdir build
cd build
cmake ..
make

ctest

cd ../sdsl-lite
./install.sh ..
