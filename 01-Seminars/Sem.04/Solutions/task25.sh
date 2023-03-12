#Solution 1

#!/bin/bash

if [[ ${#} -ne 1 ]]; then
        echo "Expected 1 argument - user name."
        exit 1
fi

username=$(grep "${1}" /etc/passwd | awk -F ':' '{print $1}')

# Check if tsuch a username was found in /etc/passwd
if [[ -z ${username} ]]; then
        echo "No matches for ${1}."
        exit 2
fi

ps -A -o user,pid,tty,pid,rss,vsz --sort -vsz | grep "^${username}" | awk '{print $5/$6}'

#=================================================================================================#

#Solution 2

#!/bin/bash

if [[ $# -ne 1 ]]; then
        echo "invalid argument count"
        exit 1
fi

if ! grep -q "$1" "/etc/passwd"; then
        echo "There is no such user"
        exit 2
fi

while read line; do
        process=$(echo "$line" | cut -d ' ' -f 1)
        physical=$(echo "$line" | cut -d ' ' -f 2)
        virtual=$(echo "$line" | cut -d ' ' -f 3)
        memoryRatio=$(echo "scale=2; $physical/$virtual" | bc)

        printf "Process: $process | Ratio: %.2f\n" "$memoryRatio"
done< <(ps -u $1 -o pid=,rss=,vsz= | sed 's/ //' | tr -s ' ' | sort -nr -t ' ' -k 3)
