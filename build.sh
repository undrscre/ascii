#!/bin/bash

# I should consider makefiles.
# Assuming you're in the working directory, of course..
mkdir -p build
cmake -S . -B build
cmake --build build
./build/ascii