#!/bin/bash

# Check if /home exists
if [[ ! -d /home ]]; then
    echo "/home directory does not exist"
    exit 1
fi

if [[ ! -r /home ]]; then
    echo "We do not have access to read in /home directory"
    exit 2
fi

# Find the most recently modified file in /home
userAndFile=$(find /home -type f -printf "%T@ %u %f\n" 2>/dev/null | sort -nr -k 1 | head -n 1 | awk '{print $2 " " $3}')

# Check if any file was found
if [[ -z $userAndFile ]]; then
    echo "No files found in /home"
    exit 2
fi

# Output the result
echo "Answer: $userAndFile"
exit 0