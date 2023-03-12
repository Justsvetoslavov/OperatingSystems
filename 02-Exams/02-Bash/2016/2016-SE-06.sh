#!/bin/bash

# Check if exactly one argument is provided
if [[ $# -ne 1 ]]; then
    echo "Invalid number of arguments"
    exit 1
fi

# Check if the argument is a valid file
if [[ ! -f "$1" ]]; then
    echo "$1 should be a file"
    exit 2
fi

file="$1"
counter=1
data=""

# Process each line in the file
while read -r line; do
    [[ -z "$line" ]] && continue  # Skip empty lines
    text=$(echo "$line" | cut -d '-' -f 2- | sed 's/ //')  # Remove leading spaces
    if [[ -n "$data" ]]; then
        data+=$'\n'  # Add newline between entries
    fi
    data+="$counter. $text"
    ((counter++))
done < "$file"  # Directly read from the file without `cat`

# Sort and display the results
echo -e "$data" | sort -k 2,2
