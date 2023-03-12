#!/bin/bash

# Warn if there are arguments
if [[ $# -gt 0 ]]; then
    echo "All arguments are redundant, thus skipped"
fi

# Check if the script is run as root
if [[ $(id -u) -ne 0 ]]; then
    echo "This script must be run as root"
    exit 1
fi

# Process /etc/passwd
while read -r line; do
    user=$(echo "$line" | cut -d ':' -f 1)
    homedir=$(echo "$line" | cut -d ':' -f 6)

    if [[ -z $homedir ]]; then
        echo "User '$user' has no home directory defined"
    elif [[ ! -d $homedir ]]; then
        echo "User '$user' does not have a home directory: $homedir"
    elif [[ ! -w $homedir ]]; then
        echo "User '$user' cannot write to their home directory: $homedir"
    fi
done < /etc/passwd