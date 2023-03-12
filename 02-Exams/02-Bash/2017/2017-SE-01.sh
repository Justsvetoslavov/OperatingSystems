#!/bin/bash

# Check argument count
if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "Invalid argument count"
    exit 1
fi

# Check if the first argument is a directory
if [[ ! -d $1 ]]; then
    echo "$1 is not a directory"
    exit 2
fi

# First argument is valid
dir=$1

# If one argument is provided, find broken symlinks
if [[ $# -eq 1 ]]; then
    brokenSymlinks=$(find "$dir" -xtype l 2>/dev/null)
    if [[ -z $brokenSymlinks ]]; then
        echo "No broken symlinks found in $dir"
    else
        echo "Broken symlinks in $dir:"
        echo "$brokenSymlinks"
    fi
    exit 0
fi

# If two arguments are provided, validate the second argument as a non-negative integer
if [[ ! $2 =~ ^[0-9]+$ ]]; then
    echo "$2 is not a non-negative integer"
    exit 3
fi

# Handle the edge case for $2 = 0
if [[ $2 -eq 0 ]]; then
    echo "Files with 1 or more hard links in $dir:"
    files=$(find "$dir" -type f -links +0 2>/dev/null)
else
    hardLinks=$(( $2 - 1 ))
    echo "Files with more than $2 hard links in $dir:"
    files=$(find "$dir" -type f -links +"$hardLinks" 2>/dev/null)
fi

# Output results or a message if no files are found
if [[ -z $files ]]; then
    echo "No files matching the criteria found in $dir"
else
    echo "$files"
fi