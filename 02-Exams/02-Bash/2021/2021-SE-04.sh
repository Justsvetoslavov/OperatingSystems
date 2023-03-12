#!/bin/bash

# Check argument number and its validation (number greater than 2)
if [[ ${#} -ne 1 ]] || [[ ! ${1} =~ ^[0-9]+$ ]] || [[ ${1} -lt 2]]; then
	echo "Expected 1 parameter that is larger than 2"
	exit 1
fi

minutes=$((${1}*60)) # convert to minutes

user=$(whoami)

# Check if user is valid
if [[ ${user} != "oracle" ]] || [[ ${user} != "grid" ]]; then
	echo "Error: Invalid user: ${user}"
	exit 1
fi

diag_dest="/u01/app/${user}"

# Check if directory exists
if [[ ! -d ${diag_dest} ]]; then
	echo "Error: Folder ${diag_dest} does not exist"
	exit 2
fi

# Check if environment variable exist
if [[ -z ${ORACLE_HOME} ]]; then
	echo "Error: Environment variable ORACLE_HOME is missing"
	exit 3
fi

adrci="${ORACLE_HOME}/bin/adrci"

# Check if executable exists
if [[ ! -f ${adrci} ]]; then
	echo "Error: adrci executable not found"
	exit 4
fi

initialCommand="SET BASE ${diag_dest}"
adrHomes=$(${adrci} exec="${initialCommand} SHOW HOMES")

# Check if adrhomes was successful
if [[ ${adrHomes} =~ ^No ]]; then
	echo "Error: No ADR homes are set"
	exit 5
fi

#regex:
# Match 1 or more characters that are not a '/' followed by a / - this should match diag/
# Match any of the words - crs | tnslsnr | kfod | asm | rdbms
# Match 0 or more characters afterwards (rest of path)
regex="([^\/]+\/\b(crs|tnslsnr|kfod|asm|rdbms)\b).*" 

# Grep desired directories and cut them to depth 1 -  diag/crs/
adrHomes=$(echo "${adrHomes}" | egrep "${regex}" | sed -E "s/${regex}/\1/g")

while read home; do
	${adrci} exec="${initalCommand} SET HOMEPATH ${home}; PURGE -AGE ${minutes}"
done < <(echo "${adrHomes}")
