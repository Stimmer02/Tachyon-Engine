#!/bin/bash

echo "Compilation in progress..."
mkdir -p build/lib/googletest/include
cmake -G "Unix Makefiles" -B build
cmake --build build
echo "Compilation done"

