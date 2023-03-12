#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Invalid argument count"
        exit 1
fi

if [[ ! -f $1 || ! -d $2 ]]; then
        echo "Invalid arguments"
        exit 2
fi

while read file; do
        diff -q ${1} ${file} > /dev/null
        if [[ $? -eq 0 ]]; then
                basename=$(basename $file)
                echo "$basename"
        fi
done< <(find $2 -type f)
