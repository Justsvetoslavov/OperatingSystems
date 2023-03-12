#!/bin/bash

# Check for number of arguments
if [[ ${#} -ne 3 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

# Check if first file/first argument is a file and exists
if [[ ! -f ${1} ]]; then
	echo "Error: ${1} is not a file"
	exit 2
fi

# Second argument/file should NOT exist
if [[ -f ${2} ]]; then
	echo "Error: ${2} must a file that does not exist"
	exit 2
fi

# Check if third argument is a directory and exists
if [[ ! -d ${3} ]]; then
	echo "Error: ${3} is not a directory"
	exit 2
fi

# We need to have read permissions for first & third argument
if [[ ! -r ${1} ]] || [[ ! -r ${3} ]]; then
	echo "Error: Missing read permission/s"
	exit 2
fi

# regex for matching lines in the files
# 1) Empty lines/Lines with only whitespaces
# 2) Comment lines (starting with '#')
# 3) Lines: { something } #something
regex="^\s*$|^#.*$|^\s*\{\s.+\s\};(\s*#.*)?"

validFiles=""

while read file; do
    # Check for invalid lines and print them if there are such
	invalidLines=$(cat "${file}" | egrep -v -n "${regex}")	
	if [[ ! -z "${invalidLines}" ]]; then
		output=$(echo "${invalidLines}" | awk -F ':' '{print "Line " $1 ":" $2}')
		echo "${output}"
		continue # File is invalid, move to next
    fi
 
   validFiles="${validFiles} ${file}" # concatenation of files 

   basename=$(basename "${file}") # Extract basename - something.cfg
   username="${basename%.*}" # remove extension - something
   if ! egrep -q ${basename} ${1}; then # Check if username is present in file
   	 pass=$(pwgen 16 1)
	 echo "${username} ${pass}" # output
   	 echo -ne "${username}:${pass}\n" >> $1 # add to file
   fi
done < <(find ${3} -type f -name "*.cfg")

cat ${validFiles} > $2
