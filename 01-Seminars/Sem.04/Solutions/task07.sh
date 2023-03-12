# Solution 1

#!/bin/bash

if [ $# -ne 3 ]; then
  echo "Invalid number of arguments"
  exit 1
fi

if [[ ! -f ${1} ]]; then
  echo "${1} is not found"
  exit 2
elif  [[ ! -f ${2} ]]; then
  echo "${2} is not found"
  exit 3
elif  [[ ! -f ${3} ]]; then
  echo"${3} is not found"
  exit 4
fi

paste ${1} ${2} | sort > ${3}

#========================================================#

# Solution 2

#!/bin/bash

read -p "Enter name of 3 files: " f1 f2 f3

if [[ ! -f $f1 || ! -f $f2 ]]; then
        echo "Invalid files entered"
        exit 1
fi

if [[ -e $f3 && ! -f $f3 ]]; then
        echo "$f3 exists, but is not a file"
        exit 2
fi

paste $f1 $f2 | sort > $f3
