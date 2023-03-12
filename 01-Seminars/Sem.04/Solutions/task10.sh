#!/bin/bash

if [[ $# -ne 3 ]]; then
        echo "Invalid argument count"
        exit 1
fi

filename=$1
name=$2
nickname=$3

if [[ ! -f $filename || -z $name || -z $nicknamе ]]; then
        echo "Invalid arguments"
        exit 1
fi

names=$(egrep -o "$name" /etc/passwd)
length=$(echo "$names" | wc -l)

if [[ $length -eq 0 ]]; then
        echo "There are no matches for $name"
        exit 2
fi

if [[ $length -eq 1 ]]; then
        echo "$name $nickname" >> $filename
        exit 0
fi

index=1
while read line; do
        echo "$index: $line"
        arr[index]=${line}
        index=$((index + 1))
done< <(echo "$names")

read -p "Please enter your choice: " choice

if [[ $choice -lt 0 || $choice -ge $index ]]; then
        echo "Your choice $choice is invalid"
        exit 3
elif [[ $choice -eq 0 ]]; then
        echo "Your choice $choice: No further operations"
        exit 0
else
        echo "${arr[$choice]} $nicknames" >> $filename
        exit 0
fi