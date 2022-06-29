#!/bin/sh

rm -r ./build/linux/*.*
mkdir ./build/linux

cmake -S . -B ./build/linux/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build/linux/debug --clean-first --config debug --target all

cmake -S . -B ./build/linux/release -DCMAKE_BUILD_TYPE=Release
cmake --build ./build/linux/release --clean-first --config release --target all
