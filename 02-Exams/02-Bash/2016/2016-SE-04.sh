#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Invalid argument count"
    exit 1
fi

if [[ ! $1 =~ ^[+-]?([1-9][0-9]*|0)$ || ! $2 =~ ^[+-]?([1-9][0-9]*|0)$ ]]; then
    echo "Invalid argument type"
    exit 2
fi

if [[ $1 -ge $2 ]]; then
    echo "Invalid interval - $1 should be less than $2"
    exit 3
fi

# Create directories if they don't already exist
# mkdir -p - does not complain if it does exist
mkdir -p a b c

# Loop through files
for file in $(find . -maxdepth 1 -type f); do 
    # Count lines in the file
    lines=$(wc -l < "$file")
    if [[ $lines -lt $1 ]]; then
        mv "$file" a/
    elif [[ $lines -lt $2 ]]; then
        mv "$file" b/
    else
        mv "$file" c/
    fi
done