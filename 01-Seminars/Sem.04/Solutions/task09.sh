#!/bin/bash

if [[ ${#} -ne 1 ]]; then
        exit 1
fi

if [[ ! -f ${1} || ! -e ${1} ]]; then
        exit 2
fi

depth=0
maxdepth=0

while read ch; do
        if [[ ${ch} == "{" ]]; then
                depth=$(( depth + 1 ))
                if [[ ${maxdepth} -lt ${depth} ]]; then
                        maxdepth=${depth}
                fi
        elif [[ ${ch} == "}" ]]; then
                depth=$(( depth - 1 ))
        fi
done < <(cat $1 | egrep -o '{}')

echo "Max depth was: ${maxdepth}"
