#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Expected 2 arguments:"
        echo "Source directory"
        echo "Destination directory"
        exit 1
fi

source="$1"
destination="$2"

if [[ ! -d "$source" ]]; then
        echo "Source directory does not exist"
        exit 1
fi

if [[ ! -d "$destination" ]]; then
        mkdir "$destination"
fi

for file in $(find "${source}" -mindepth 1 -type f); do
        extension=$(echo "${file}" | awk -F '.' '{print $NF}')
        echo "${destination}/${extenstion}"
        if [[ ! -d "${destination}/${extension}" ]]; then
                echo "Make"
                mkdir "${destination}/${extension}"
        fi
        cp "${file}" "${destination}/${extension}"
done