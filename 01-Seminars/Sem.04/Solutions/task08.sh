# Solution 1

#!/bin/bash

read -p "Enter a file" file
read -p "Enter a string" str

if [[ ! -e ${file} || ! -f  ${file} || -z ${str} ]]; then
        echo "Error"
        exit 1
fi

grep -q "${str}" ${file}

exit_code=$?
echo "Code is: ${exit_code}"

#========================================================#

# Solution 2

#!/bin/bash

if [[ $# -ne 2 ]]; then
        echo "Invalid number of arguments"
        exit 1
fi

if [[ ! -f $1 || -z $2 ]]; then
        echo "Invalid arguments"
        exit 2
fi

grep -q "$2" $1
echo "Exit status: $?"