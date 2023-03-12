#!/bin/bash

if [[ $# -lt 1 || $# -gt 2 ]]; then
        echo "Expected 1st arg -> dir name"
        echo "Optional 2nd arg: -a"
        exit 1
fi

if [[ ! -d $1 ]]; then
        echo "Not a directory"
        exit 2
fi

if [[ $2 == "-a" ]]; then
        echo "I am in -a option"
        find_opts="-mindepth 1 -maxdepth 1"
else
        find_opts="-mindepth 1 -maxdepth 1 ! -name '.*'"
fi

for file in $(eval find $1 $find_opts); do
        if [[ -d $file ]]; then
                echo "$file ($(find $file $find_opts | wc -l) entries)"
        elif [[ -f $file ]] ; then
                echo "$file $(stat -c "%s" $file)b size"
        fi
done
