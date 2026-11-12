#!/bin/sh
set -e

rm -rf build

cmake -B build
cmake --build build

# check headers for warnings
cmake --build build --target check_headers

./build/gi_artifact_engine