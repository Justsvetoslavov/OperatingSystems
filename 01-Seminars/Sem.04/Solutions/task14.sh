#!/bin/bash

if [[ $# -eq 0 ]] ; then
        echo "Expected at least 1 argument"
        exit 1
fi

if [[ $# -gt 2 ]] ; then
        echo "Expected #1 argument - number"
        echo "Optional #2 argument - separator"
        exit 1
fi

number=${1}

echo "${number}" | egrep -q "^(-){0,1}[0-9]+$"
if [[ ${?} -ne 0 ]] ; then
        echo "${number} is not integer"
        exit 1
fi

separator=" "

if [[ $# -eq 2 ]] ; then
        separator="${2}"
fi

if [[ $(echo "${separator}" | egrep -o "." | wc -l) -ne 1 ]] ; then
        echo "Separator should be 1 character"
        exit 1
fi

number=$(echo "${number}" | egrep -o ".")

length=$(echo "${number}" | wc -l)
counter=$(( length % 3 ))
counter=$(( 3 - counter ))

while read digit ; do
        counter=$(( counter + 1 ))
        length=$(( length - 1 ))
        printf "%s" "${digit}"
        if [[ $(( counter % 3 )) -eq 0 ]] && [[ ${length} -ne 0 ]] ; then
                printf "%s" "${separator}"
        fi
done < <(echo "${number}")
printf "\n"