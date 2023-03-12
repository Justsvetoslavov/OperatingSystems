#!/bin/bash

user=$(whoami)

# Check user
if [[ ${user} != "oracle" ]] || [[ ${user} != "grid" ]]; then
	echo "Error: Invalid user: ${user}"
	exit 1
fi

# Check environment variable
if [[ -z ${ORACLE_HOME} ]]; then
	echo "Error: ORACLE_HOME not found!"
	exit 2
fi

adrci="${ORACLE_HOME}/bin/adrci"

# Check for executable file
if [[ ! -f ${adrci} ]]; then
	echo "Error: adrci executable not found"
	exit 3
fi

diag_dest="/u01/app/${user}"

adrHomes=$(${adrci} exec="show homes")

# Check if it starts with "No" (option 2 to check for whole string)
if [[ ${adrHomes} ~= ^No ]]; then
	echo "No ADR homes found"
	exit 4
fi

adrHomes=$(echo "${adrHomes}" | tail -n +2)

while read home; do
	home=$(echo "${home}" | sed -E -e 's/^\s+//' -e 's/\s+$//')
	path="${diag_dest}/${home}"

	bytes=$(find ${path} -printf '%s\n')
	
	kb=$((${bytes}/1024))
	mb=$((${kb}/1024))

	echo "${mb} ${path}"
done < <(echo "{$adrHomes}"
