#!/bin/bash

if [[ ${#} -ne 1 ]] || [[ ! -f ${1} ]] ; then
        echo "Expected 1 argument - config file"
        exit 1
fi

devices=$(cat /proc/acpi/wakeup | tail -n +2 | awk '{print $1 " " $3}')
file="${1}"

while read device status ; do
        echo "${devices}" | grep -q "^${device}"
        if [[ ${?} -ne 0 ]] ; then
                echo "Warning, device ${device} does not exist"
        else
                device_status=$(echo "${devices}" | grep "^${device}" | awk '{print $2}')
                if [[ ${device_status} != "disabled" ]] && [[ ${device_status} != "enabled" ]] ; then
                        echo "Invalid device status"
                elif [[ ${device_status} != ${status} ]] ; then
                        echo "${device}" >> "/proc/acpi/wakeup"
                fi
        fi
done < <(cat ${file} | tail -n +2 | awk -F '#' '{print $1}')