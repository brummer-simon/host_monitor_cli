#!/bin/bash

# Cleanup
[[ -e build-debug ]]   && rm -rf build-debug
[[ -e build-release ]] && rm -rf build-release

# Build Debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -B build-debug -S .     && make -C build-debug all

# Build Release and install it
cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -B build-release -S . && make -C build-release all && sudo make -C build-release install
