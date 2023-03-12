#!/bin/bash

if [[ $# -ne 1 ]] ; then
        echo "Expected 1 argument - directory"
        exit 1
fi

if [[ ! -d ${1} ]] ; then
        echo "Expected 1 argument - directory"
        exit 1
fi

for file in $(find "${1}" -mindepth 1 -maxdepth 1 -type f); do
        if [[ -f "${file}" ]] ; then
                for FILE in $(find "${1}" -mindepth 1 -maxdepth 1 -type f) ; do
                        if [[ -f "${FILE}" ]] && [[ -f "${file}" ]] && [[ "${file}" != "${FILE}" ]]; then
                                diff -q "${file}" "${FILE}" >/dev/null
                                if [[ ${?} -eq 0 ]] ; then
                                        if [[ "${file}" > "${FILE}" ]] ; then
                                                rm "${file}"
                                        else
                                                rm "${FILE}"
                                        fi
                                fi
                        fi
                done
        fi
done