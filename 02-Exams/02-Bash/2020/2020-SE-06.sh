#!/bin/bash

if [[ ${#} -ne 3 ]]; then
	echo "Error: Invalid number of arguments"
	exit 1
fi

if [[ ! -f ${1} ]]; then
	echo "Error: ${1} has to exist and to be a file"
	exit 1
fi

File="${1}"
key=${2}
value=${3}
date=$(date)
user=$(whoami)

comment="# edited at ${date} by ${user}"

line=$(egrep "^\s*${key}" ${File})
# TODO: If we have more than 1 key match, this will not work properly

newline="${key}=${value} ${comment}"


echo "Line: ${line}"
if [[ -z "${line}" ]]; then
	echo "${newline}" >> ${File}
	exit 0;
fi

valueInFile=$(echo "${line}" | cut -d '#' -f 1 | cut -d '=' -f 2 | xargs)
echo "Value in file: ${valueInFile}"
echo "Value: ${value}"

if [[ ${valueInFile} == ${value} ]]; then
	echo "Value already exitsts"
	exit 2
fi

# Use sed
# -i for file in-lace modification
# -E for extended regex
# /${line}/ - pattern to search
# { ... } - block of sed commands that will be applied to each line that matches the pattern
# 's/^/# / - substitution command. Replace the start of the line with '# '
# s/$/ $ edited at date by user - substitution command for the end
# a - stands for append
# a ${key} = ${value} ... - appends 
sed -i -E "/${line}/{
		s/^/# /
		s/$/ # edited at ${date} by ${user}/
		a ${key} = ${value} # added at ${date} by ${user}
	}" ${File}