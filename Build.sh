#!/bin/bash

echo "Compilation in progress..."
cmake -G "Unix Makefiles" -B build
cmake --build build
echo "Compilation done"

