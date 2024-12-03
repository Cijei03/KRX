#!/bin/bash

mkdir build_cache
rm -rf ./build_cache/*
mkdir bin
rm -rf ./bin/*

cd ./build_cache
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja

cd ..