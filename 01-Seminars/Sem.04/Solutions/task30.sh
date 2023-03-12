#!/bin/bash

rec=1

if [[ "${1}" == "-r" ]] ; then
        rec=0
        shift
fi

while [[ $# -ne 0 ]] ; do
        file="${1}"
        currentDate=$(date +'%Y-%m-%d-%H-%M-%S')
        if [[ -f "${file}" ]] ; then
                echo "[${currentDate}] Removed file ${file}"
                tar -czf "${BACKUP_DIR}/${file}_${currentDate}.gz" "${file}"
                rm ${file}
        elif [[ -d "${file}" ]] ; then
                dirName=$(echo "${file}" | sed -E 's/(.*)\//\1/g')
                filesCount=$(find "${file}" -mindepth 1 | wc -l)
                if [[ ${filesCount} -eq 0 ]] ; then
                        echo "[${currentDate}] Removed directory ${file}"
                        tar -czf "${BACKUP_DIR}/${dirName}_${currentDate}.tgz" "${file}"
                        rmdir ${file}
                else
                        if [[ ${rec} -eq 0 ]] ; then
                                echo "[${currentDate}] Removed directory recursively ${file}"
                                tar -czf "${BACKUP_DIR}/${dirName}_${currentDate}.tgz" "${file}"
                                rm -rf ${file}
                        else
                                echo "error: ${file} is not empty, will not detele"
                        fi
                fi
        fi
        shift
done