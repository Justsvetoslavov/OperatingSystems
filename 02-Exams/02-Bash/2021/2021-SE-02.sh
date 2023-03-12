#!/bin/bash

# Note: This solution was taken
# It was only reviewed in terms of logic.

function collapse {
        in_braces=no

        while read line; do
                if echo "${line}" | grep -q -E "\($"; then
                        in_braces=yes
                        echo "${line}" | sed -E 's/\($//g' | tr -d '\n'
                elif echo "${line}" | grep -q -E "^\)"; then
                        in_braces=no
                        echo "${line}" | sed -E 's/^\)//g'
                elif [[ "${in_braces}" == yes ]]; then
                        echo -n " ${line}"
                else
                        echo "${line}"
                fi
        done < <(cat "${1}" | sed 's/\s*;.*//g')
}

function get_serial_num {
        collapse "${1}" | awk '
                $2 == "IN" && $3 == "SOA" { print $6 }
                $3 == "IN" && $4 == "SOA" { print $7 }
                '
}

function increase_serial {
        old_date="$(echo "${1}" | cut -c 1-8)"
        old_num="$(echo "${1}" | cut -c 9-10 | sed 's/^0*//g')"
        today="$(date +'%Y%m%d')"

        if [[ "${old_date}" == "${today}" ]]; then
                if [[ "${old_num}" -eq 99 ]]; then
                        echo "error processing ${2}: cannot increase 99" >&2
                        return 1
                fi

                new_num=$(( old_num + 1 ))
                new_date="${old_date}"
        elif [[ "${old_date}" < "${today}" ]]; then
                new_num=1
                new_date="${today}"
        else
                echo "error processing ${2}: serial number is from the future" >&2
                return 1
        fi

        new_num="$(printf '%02d\n' "${new_num}")"

        echo "${new_date}${new_num}"
}

function bump_serial {
        if ! { [[ -r "${1}"  ]] && [[ -w "${1}" ]] }; then
                echo "cannot process ${1}: cannot read or write" >&2
                exit 1
        fi

        old_serial="$(get_serial_num "${1}")"
        if [[ -z "${old_serial}" ]]; then
                echo "cannot process ${1}: cannot find SOA record" >&2
                exit 1
        fi


        new_serial="$(increase_serial "${old_serial}" "${1}")"
        if [[ $? -ne 0 ]]; then
                return 1
        fi

        sed -i "s/${old_serial}/${new_serial}/g" "${1}"
}

if [[ ${#} -eq 0 ]]; then
        echo "${0} bumps the serial numbers of zone files"
        echo "usage: ${0} <one or more zone files>"
        exit 1
fi

for file in "${@}"; do
        bump_serial "${file}"
done