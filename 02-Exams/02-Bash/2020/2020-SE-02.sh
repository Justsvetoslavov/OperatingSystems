#!/bin/bash

# Check if we have only 1 argument 
if [[ ${#} -ne 1 ]]; then
	echo "Error: Invalid number of arguments"
	exit 1
fi

# Check if is a file
if [[ ! -f ${1} ]]; then
	echo "Error: ${1} is not a file!"
	exit 1
fi

# Check if we can read from file
if [[ ! -r ${1} ]]; then
	echo "Errro: Cannot read from file ${1}"
	exit 1
fi

File="${1}" # create variable

topSites=$(cat ${File} | cut -d ' ' -f 2 | uniq -c | sort -nr | tr -s ' ' | sed 's/ //' | cut -d ' ' -f 2 | head -n 3)

while read -r site; do
	siteOccurances=$(cat ${File} | grep -E "${site}")
	sitesHTTP=$(echo ${siteOccurances} | grep -E "HTTP/2.0" | wc -l)
	sitesNonHTTP=$(echo ${siteOccurances} | grep -v "HTTP/2.0" | wc -l)
	echo "HTTP 2.0 sites count: ${sitesHTTP}"
	echo "Non-HTTP sites count: ${sitesNonHTTP}"
	cat ${File} | grep -E "${site}" | awk '{if($(NF-5)>302) print $1}' | sort | uniq -c | sort -nr | tr -s ' ' | sed 's/ //' | head -n 5
done < <(echo "${topSites}")

