# Solution if we provide a directory as a parameter, rather than from $PATH

#!/bin/bash

if [[ $# -ne 1 ]] || [[ ! -d ${1} ]]; then
        echo "Expected 1 argument - directory"
        exit 1
fi

counter=0

for file in $(find ${1} -mindepth 1 -maxdepth 1 -type f) ; do
        if [[ -x ${file} ]] ; then
                counter=$(( counter + 1 ))
        fi
done

echo "${counter}"

#=================================================================================================#

# Solution of task

#!/bin/bash

# Initialize the counter for executable files
exec_count=0

# Iterate over directories in PATH
while IFS= read -rd: dir; do
    # Skip non-existent directories
    [[ -d "$dir" ]] || continue
    # Add the count of executable files in the directory
    exec_count=$((exec_count + $(find "$dir" -maxdepth 1 -type f -perm -u=x 2>/dev/null | wc -l)))
done <<< "$PATH:"

# Output the result
echo "Total number of executable files in PATH: $exec_count"
