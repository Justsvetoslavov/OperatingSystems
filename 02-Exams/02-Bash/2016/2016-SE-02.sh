#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Invalid argument count"
    exit 1
fi

if ! [[ $1 =~ ^[0-9]+$ ]]; then
    echo "$1 is not a valid positive integer"
    exit 2
fi

number=$1

if [[ $(whoami) != "root" ]]; then
    echo "We are not root. Exiting ..."
    exit 3
fi

while read userId; do
    data=$(ps -e -o uid=,pid=,rss= | awk -v userId="$userId" '$1 == userId')
    largestProcess=$(echo "$data" | sort -k3 -nr | head -n1 | awk '{print $2}')
    sumOfMemory=$(echo "$data" | awk '{sum+=$3} END{print sum}')

    echo "Sum of memory for $userId is $sumOfMemory"

    if [[ $sumOfMemory -gt $number ]]; then
        if [[ -n "$largestProcess" ]]; then
            echo "Terminating process $largestProcess"
            kill "$largestProcess"
        else
            echo "No process found to terminate for $userId"
        fi
    fi
done < <(ps -e -o uid= --sort uid | uniq)
