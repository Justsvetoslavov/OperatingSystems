#!/bin/bash

while [ $# -ne 0 ]; do
    if [[ -f $1 ]]; then
        if [[ -r $1 ]]; then
            echo "$1 is a file and we are able to read it"
        else
            echo "$1 is a file, but we CANNOT read it"
        fi
    elif [[ -d $1 ]]; then
        echo "$1 is a directory."

        # Get the file count and file details in one step
        files=$(find "$1" -mindepth 1 -maxdepth 1 -type f -printf "%s %f\n")
        fileCount=$(echo "$files" | wc -l)

        echo "Files in $1:"
        while read -r filesize filename; do
            if [[ $filesize -lt $fileCount ]]; then
                echo "$filename"
            fi
        done <<< "$files"
    else
        echo "Error: $1 is not a file or directory!"
    fi

    # Move to the next argument
    shift
done
