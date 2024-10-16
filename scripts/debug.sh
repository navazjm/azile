#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build/debug

make -C build/debug

# Run the executable if the build was successful
if [ $? -eq 0 ]; then
    echo "Debug - Build succeeded!"
else
    echo "Debig - Build failed!"
    exit 1
fi

