#!/bin/bash

if [[ $# -gt 2 ]] || [[ $# -lt 1 ]] ; then
        echo "Expected 2 arguments - color and string"
        exit 1
fi

if [[ $# -eq 1 ]] ; then
        echo "${1}"
        exit 0
fi

color="${1}"
shift

if [[ ${color} == "-r" ]] ; then
        echo -e "\033[0;31m${@}"
elif [[ ${color} == "-g" ]] ; then
        echo -e "\033[0;32m${@}"
elif [[ ${color} == "-b" ]] ; then
        echo -e "\033[0;34m${@}"
else
        echo "Unknown color"
fi

echo -e '\033[0m'
