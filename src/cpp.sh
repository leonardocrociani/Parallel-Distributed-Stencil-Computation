#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file.cpp> [args...]"
    exit 1
fi

file=$1
shift
base_name=$(basename "$file" .cpp)

make clean && make -j "$base_name" && ./"$base_name".o "$@"