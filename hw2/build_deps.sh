#!/bin/bash

cd FastaReader
mkdir build
cd build
cmake ..
make

ctest
