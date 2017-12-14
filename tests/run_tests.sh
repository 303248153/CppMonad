#!/usr/bin/env bash
mkdir -p Build
cd Build
cmake ../ && \
make && \
./CppMonadTest
