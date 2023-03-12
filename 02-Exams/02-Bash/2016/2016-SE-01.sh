#!/bin/bash

if [[ $# -ne 1 ]]; then
        echo "Invalid argument count"
        exit 1
fi

if [[ ! -d $1 ]]; then
        echo "Argument should be a directory"
        exit 2
fi

dir=$1

brokenSymlinks=$(find $dir -xtype l -printf "%f\n" 2>/dev/null) # or %p if we want fullpath
echo "$brokenSymlinks"
