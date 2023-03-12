#Solution 1

#!/bin/bash

if [[ $# -ne 2 ]] ; then
        echo "Expected 2 arguments - directory and number"
        exit 1
fi

if [[ ! -d "${1}" ]] ; then
        echo "First argument is not a directory"
        exit 1
fi

echo "${2}" | egrep -q "^[0-9]+$"
if [[ ${?} -ne 0 ]] ; then
        echo "Second argument is not an integer"
        exit 1
fi

for file in $(find ${1} -mindepth 1 -maxdepth 1 -type f) ; do
        fileSize=$(stat -c "%s" ${file})
        if [[ ${fileSize} -gt ${2} ]] ; then
                echo "${file}"
        fi
done

#=================================================================================================#

# Solution 2

#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Invalid argument count"
        exit 1
fi

if [[ ! -d $1 || ! $2 =~ ^[0-9]+$ ]]; then
        echo "Invalid arguments"
        exit 2
fi

dir=$1
number=$2

while read filedata; do
        filename=$(echo $filedata | cut -d ' ' -f 2)
        filesize=$(echo $filedata | cut -d ' ' -f 1)

        if [[ $filesize -gt $number ]]; then
                echo "$filename"
        fi
done< <(find $dir -mindepth 1 -maxdepth 1 -type f -printf "%s %f\n")
