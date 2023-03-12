#!/bin/bash

# Validate arguments
if [[ ${#} -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi
if [[ ! -d ${1} ]]; then
	echo "Error: ${1} is not a directory"
	exit 2
fi
if [[ -e ${2} ]]; then
	echo "Error: "${2}" already exists"
	exit 3
fi

if [[ ! -r ${1} ]]; then
	echo "Error: Missing read permissions for ${1}"
	exit 4
fi

SRC="${1}"
DST="${2}"

mkdir -p "${DST}"

while read -r file; do
	basename=$(basename "${file}")
	name=$(echo "${basename}" | sed 's/([^()]*)//g') # remove strings enclosed in brackets
	name="${name%.*}" # remove extension
	name=$(echo "${name}" | xargs) # remove leading and trailing whitespaces
	album=$(echo "${basename}" | grep -o '(\([^)]*\))' | awk 'END{print}')
	date=$(find "${file}" -printf '%TF\n')
	sha=$(sha256sum "${file}" | cut -d ' ' -f 1)

	# Test output
	# echo -e "${basename}\n${name}\n${album}\n${date}\n${sha}"

	mkdir -p "${DST}"
	mkdir -p "${DST}"/by-date/"${date}"/by-album/"${album}"/by-title
	mkdir -p "${DST}"/by-date/"${date}"/by-title
	mkdir -p "${DST}"/by-album/"${album}"/by-date/"${date}"/by-title
	mkdir -p "${DST}"/by-album/"${album}"/by-title
	mkdir -p "${DST}"/by-title
	mkdir -p "${DST}"/images

	cp "${file}" "${DST}"/images/"${sha}".jpg

	ln -s "${DST}"/images/"${sha}".jpg "${DST}"/by-date/"${date}"/by-album/"${album}"/by-title/"${name}".jpg
    ln -s "${DST}"/images/"${sha}".jpg "${DST}"/by-date/"${date}"/by-title/"${name}".jpg
    ln -s "${DST}"/images/"${sha}".jpg "${DST}"/by-album/"${album}"/by-date/"${date}"/by-title/"${name}".jpg
    ln -s "${DST}"/images/"${sha}".jpg "${DST}"/by-album/"${album}"/by-title/"${name}".jpg
    ln -s "${DST}"/images/"${sha}".jpg "${DST}"/by-title/"${name}".jpg
done< <(find "${SRC}" -mindepth 1 -type f -name "*.jpg")

