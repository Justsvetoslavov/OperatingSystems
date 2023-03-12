#!/bin/bash

if [[ ${#} -ne 2 ]]; then
	echo "Error: Invalid number of arguments"
	exit 1
fi

# Check if binary files exists
if [[ ! -f ${1} ]]; then
	echo "Error: ${1} does not exist or is not a file"
	exit 2
fi

file_size=$(($(stat -c %s "${1}") / 2)) # get file size of binary file
max_size=524288 # max size for binary file

if [[ ${file_size} -gt ${max_size} ]]; then
	echo "Error: ${1} is too large. ${1} is ${file_size} bytes"
	exit 3
fi

if [[ -f ${2} ]]; then
	echo "Error: ${2} already exists"
	exit 4
fi

input=${1} # binary file
output=${2} # header file

# append all necessary headers and preprocessor directives
# Todo - preprocessor directives
echo "#include <stdlib.h>" > ${output} # Create or Trunc file + append header

Arr=""
counter=0

for UINT16 in $(xxd -p -c 2 ${input}); do
	UINT16=$(echo "${UINT16}" | sed -E 's/^(..)(..)$/\2\1/') # swap bytes due to endiness
	ARR="${ARR}0x${UINT16}," # append UINT16 to ARR

	((counter++))
done

ARR=$(echo "${ARR}" | sed -E 's/,$//')

echo "uint16_t arr[] = { ${ARR} };" >> ${output} 
echo "uint32_t arrN = ${counter} ;" >> ${output}
