#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Invalid argument count"
        exit 1
fi

# Resolve full paths for the input files
lfile="$HOME/$1"
rfile="$HOME/$2"

if [[ ! -f $lfile || ! -f $rfile ]]; then
        echo "Invalid file type"
        exit 2
fi

# Extract performer names (file names without extensions)
lperformer=$(basename "$lfile")
rperformer=$(basename "$rfile")

# Count lines containing the performer's name (exact word match)
lfiledata=$(grep -w -c "$lperformer" "$lfile")
rfiledata=$(grep -w -c "$rperformer" "$rfile")

if [[ $lfiledata -gt $rfiledata ]]; then
    file="$lfile"
    performer="$lperformer"
elif [[ $lfiledata -lt $rfiledata ]]; then
    file="$rfile"
    performer="$rperformer"
else
    echo "Files have same amount of lines"
    exit 3
fi

# Extract and process content
output_file="$performer.songs"
grep "$performer" "$winner_file" | cut -d '-' -f 2 | sed 's/ //' | sort > "$output_file"

echo "Processed content saved in $output_file."