#!/bin/bash

if [[ ${#} -ne 1 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

if [[ ! ${1} =~ ^[A-Z0-9]{,4}$ ]]; then
	echo "Error: Invalid argument"
	exit 2
fi

device=${1}
file="/proc/acpi/wakeup"

if [[ ! -f ${file} ]]; then
	echo "File: wakeup is not found"
	exit 3
fi

device_line=$(grep -E "^${device}" "${file}" | tr -s ' ')

if [[ -z ${device_line} ]]; then
	echo "Device is not found"
	exit 4
fi

device_name=$(echo "${device_line}" | cut -d ' ' -f 1)
device_status=$(echo "${device_line}" | cut -d ' ' -f 3)

if [[ ${device_status} == "*disabled" ]]; then
	echo "Device is already disabled"
else
    echo "${1}" > "${file}"
fi