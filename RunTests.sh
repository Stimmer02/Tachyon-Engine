#!/bin/bash

echo "Running tests in progress..."
cd build
cd tests
ctest
echo "Done"

