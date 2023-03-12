#!/bin/bash

processes=""
counter=0

while true; do
    commands=$(ps -e -o comm=,rss=)
    if [[ -z ${commands} ]]; then
        break
    fi

    processes=$(echo "${commands}" | tr -s " " | sort -k 2 -nr | awk '$NF > 65536 {print $NF "\n"}')
    ((counter++))
    sleep 1
done

if [[ ${counter} -eq 0 ]]; then
    exit 0
fi

output=$(echo "${processes}" | sort | uniq | tr -s " " | sed 's/ //' | awk -v count="${counter}" '$1 > (count / 2) {print $2}')

echo "${output}"