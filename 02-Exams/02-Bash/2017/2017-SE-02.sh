#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Invalid number of arguments"
    exit 1
fi

# Validate arguments
if [[ ! -d $1 || ! -d $2 || -z $3 ]]; then
    echo "Invalid argument type"
    exit 2
fi

user=$(whoami)
if [[ $user != "root" ]]; then
    echo "$user is not 'root'"
    exit 3
fi

SRC=$1
DST=$2
substr=$3

# Use `find` to locate files, process them line by line
find "$SRC" -mindepth 1 -type f -regex ".*$substr.*" -printf "%p %P\n" | while read -r fullpath relpath; do
    # Create necessary directories in the destination
    mkdir -p "$DST/$(dirname "$relpath")"
    # Move the file to the destination while preserving the directory structure
    mv "$fullpath" "$DST/$relpath"
done