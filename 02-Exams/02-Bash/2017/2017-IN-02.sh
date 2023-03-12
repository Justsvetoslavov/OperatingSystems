#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Invalid argument count"
    exit 1
fi

foo=$1
user=$(whoami)

# Ensure the script is run as root
if [[ $user != "root" ]]; then
    echo "The script must be run as root"
    exit 2
fi

# Check if the user FOO exists
if ! id "$foo" &>/dev/null; then
    echo "User $foo does not exist"
    exit 3
fi

# Part a) Users with more processes than FOO
fooProcessesCount=$(ps -u "$foo" | wc -l)

echo "Users with more processes than $foo:"
while read -r username; do
    userProcessCount=$(ps -u "$username" | wc -l)
    if [[ $userProcessCount -gt $fooProcessesCount ]]; then
        echo "$username"
    fi
done < <(ps -e -o user= | sort | uniq)

# Part b) Average time (in HH:MM:SS)
# Note: An alternative (and probably simplier solution) can be:
#       to use while read hour min sec; do
#       (...)
#       done< <(ps -e -o time= | ts ':' ' ')
#       We can also format with date -u -d "$seconds" +"%H%M%S"
totalTime=$(ps -e -o etimes= | awk '{sum += $1} END {print sum}')
processCount=$(ps -e | wc -l)
if [[ $processCount -eq 0 ]]; then
    echo "No processes found to calculate average time"
    exit 4
fi

averageTime=$(awk -v total="$totalTime" -v count="$processCount" 'BEGIN {print int(total / count)}')

# Convert to HH:MM:SS
hours=$((averageTime / 3600))
minutes=$(( (averageTime % 3600) / 60 ))
seconds=$((averageTime % 60))
formattedTime=$(printf "%02d:%02d:%02d" "$hours" "$minutes" "$seconds")
echo "Average time of processes: $formattedTime"

# Part c) Kill processes of FOO that exceed twice the average time
echo "Checking and terminating processes of $foo exceeding twice the average time..."
doubleAverageTime=$((averageTime * 2))

while read -r pid; do
    echo "Terminating process $pid of user $foo"
    kill -9 "$pid"
done < <(ps -u "$foo" -o pid=,etimes= | awk -v limit="$doubleAverageTime" '$2 > limit {print $1}')

echo "Script completed."
