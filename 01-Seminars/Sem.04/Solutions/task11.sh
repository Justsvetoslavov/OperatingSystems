#!/bin/bash

if [[ ${#} -lt 1 || ${#} -gt 2 ]]; then
        echo "Invalid argument count"
        exit 1
fi

if [[ ! -d ${1} ]]; then
        exit 2
fi

sourceDir=$1
if [[ ${#} -eq 2 ]]; then
        destDir=$2
else 
        destDir=$(date -I)

if [[ ! -d ${destDir} ]]; then
        mkdir -p ${destDir}
fi

for file in $(find $sourceDir -mindepth 1 -type f -mmin -45) ; do
        cp $file $destDir
done
