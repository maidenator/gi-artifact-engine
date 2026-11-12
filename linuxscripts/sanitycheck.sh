#!/bin/sh
set -e

cmake -B build
cmake --build build --target sanity_check
./build/sanity_check