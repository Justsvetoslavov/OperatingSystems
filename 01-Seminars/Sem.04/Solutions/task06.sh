# Solution 1:

#!/bin/bash

if [[ ${#} -ne 1 ]]; then
        echo "Error: too many arguments:"
        exit 1
fi

if [[ ! -d ${1} ]]; then
        echo "Error: No such directory"
        exit 2
fi

fileCount=$(find "${1}" -type f 2>&1 | wc -l)
dirCount=$(find "${1}" -mindepth 1 -type d 2>&1 | wc -l)

echo "File count: ${fileCount}"
echo "Directory count: ${dirCount}"
echo "Total count: $(( fileCount + dirCount ))"

#========================================================#

# Solution 2

#!/bin/bash

read -p "Enter a directory: " input

if [[ -z $input ]]; then
        echo "Directory cannot be empty/blank"
        exit 1
fi

if [[ ! -d $input ]]; then
        echo "$input should be a valid directory."
        exit 2
fi

fileCount=$(find $input -mindepth 1 -type f 2>&1 | wc -l)
dirCount=$(find $input -mindepth 1 -type d 2>&1 | wc -l)

echo -e "Information for $input directory:\nFile count: $fileCount\nDirectory count: $dirCount"
