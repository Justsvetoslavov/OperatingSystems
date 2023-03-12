#!/bin/bash

rec=1

if [[ "${1}" == "-r" ]]; then
        rec=0
        shift
fi

while [[ ${#} -ne 0 ]] ; do
        file="${1}"
        currentDate=$(date +'%Y-%m-%d %H:%M:%S')
        if [[ -f "${file}" ]] ; then
                echo "[${currentDate}] Removed file ${file}"
                rm ${file}
        elif [[ -d "${file}" ]] ; then
                filesCount=$(find "${file}" -mindepth 1 | wc -l)
                if [[ ${filesCount} -eq 0 ]] ; then
                        echo "[${currentDate}] Removed directory ${file}"
                        rmdir ${file}
                else
                        if [[ ${rec} -eq 0 ]] ; then
                                echo "[${currentDate}] Removed directory recursively ${file}"
                                rm -rf ${file}
                        fi
                fi
        fi
        shift
done