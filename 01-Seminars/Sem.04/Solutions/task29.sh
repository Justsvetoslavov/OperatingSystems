#!/bin/bash

if [[ $# -ne 1 ]] ; then
        echo "Expected 1 argument - color"
        exit 1
fi

color="${1}"
index=x

if [[ ${color} == "-r" ]] ; then
        index=0
elif [[ ${color} == "-g" ]] ; then
        index=1
elif [[ ${color} == "-b" ]] ; then
        index=2
elif [[ ${color} != "-x" ]] ; then
        echo "Unknown color"
        exit 1
fi

while true ; do
        echo -e "\033[0m"
        read -p "Enter string: " str
        if [[ ${str} == "exit" ]] ; then
                echo -e '\033[0m'
                exit 0
        fi
        while read line ; do
                if [[ ${index} == "x" ]] ; then
                        echo -e "\033[0m${line}"
                elif [[ $(( index % 3 )) -eq 0 ]] ; then
                        echo -e "\033[0;31m${line}"
                elif [[ $(( index % 3 )) -eq 1 ]] ; then
                        echo -e "\033[0;32m${line}"
                elif [[ $(( index % 3 )) -eq 2 ]] ; then
                        echo -e "\033[0;34m${line}"
                fi
                if [[ ${index} != "x" ]] ; then
                        index=$(( index + 1 ))
                fi
        done < <(cat ${str})
done

echo -e '\033[0m'