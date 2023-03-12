# Solution 1:

#!/bin/bash

if [[ ${#} -ne 3 ]]; then
        exit 4;
fi

if [[ ! ${1} =~ ^[0-9]+$ || ! ${2} =~ ^[0-9]+$ || ! ${3} =~ ^[0-9]+$ ]]; then
        exit 3
fi

if [[ ${2} -gt ${3} ]]; then
        exit 2
fi

if [[ ${1} -ge ${2} && ${1} -le ${3} ]]; then
        exit 0
else
        exit 1
fi

#=================================================================================================#

#Solution 2:

#!/bin/bash

if [[ $# -ne 3 ]]; then
        echo "Invalid argument count"
        exit 4
fi

regex="^[0-9]+$"
if [[ ! $1 =~ $regex || ! $2 =~ $regex || ! $3 =~ $regex ]]; then
        echo "All arguments should be integer numbers"
        exit 3
fi

if [[ $2 -gt $3 ]]; then
        echo "Interval is incorrect"
        exit 2
fi

if [[ $1 -lt $2 || $1 -gt $3 ]]; then
        echo "Number is not in interval"
        exit 1
else
        echo "Interval is [$2;$3] and $1 is inside interval"
        exit 0
fi
