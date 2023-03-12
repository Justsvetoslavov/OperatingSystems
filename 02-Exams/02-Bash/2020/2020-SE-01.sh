#!/bin/bash

# check if exactly 2 arguments are provided
if [[ ${#} -ne 2 ]]; then
	echo "Error: Invalid number of arguments"
	exit 1
fi

File="${1}"
Dir="${2}"

# check if the specified directory exists
if [[ ! -d ${Dir} ]]; then
	echo "Directory ${Dir} does not exist"
	exit 2
fi

# Check if we have read permissions for the directory
if [[ ! -r ${Dir} ]]; then
	echo "No read permissions for directory: ${Dir}"
	exit 3
fi

# Check if the CSV file already exists
if [[ -f ${File} ]]; then
	read -p "File ${File} already exists. OverWrite? (y/n) " choice
	case ${choice} in
		y|Y ) echo "Overwriting ${File}";;
		n|N) echo "Exiting script"; exit;;
		* ) echo "Invalid response. Exiting script."; exit;;
	esac
	
	# Check if we have write permissions for the file
	if [[ ! -w ${File} ]]; then
		echo "No write permissions for file: ${File}"
		exit 3
	fi
fi

# Header for the CSV file
echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > "${File}"

while read -r log_file; do
    baseName=$(basename "${log_file}" .log) #or: baseName=$(echo "${log_file}" | rev | cut -d '/' -f 1 | rev | cut -d '.' -f 1)
    field="${baseName}"
    while read -r line; do
        field+=$(echo "${line}" | awk -F ':' '{printf "%s,",$2}')
    done < <(sed -E 's/(This platform has a[n]{0,1})(.*)/\1: \2/' "${log_file}" | sed -E 's/license[.]$//' | egrep ":")
    echo "${field}" >> "${File}"
done < <(find ${Dir} -mindepth 1 -type f -name "*.log")

exit 0

#or (Solution by Radoslav)

#!/bin/bash

if [ $# -ne 2 ]; then
  echo "ERROR: Arg count"
  exit 1
fi

if [ ! -d $2 ]; then
  echo "ERROR: Expected dir as second arg"
  exit 1
fi

CSV=$1
touch $CSV

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > $CSV

while read FILE; do
  HOST=$(basename $FILE | sed -E "s/^(.*).log$/\1/")
  LINE=$(head -n 8 $FILE | tail -n 7 | cut -d':' -f 2 | xargs | tr ' ' ',')
  LICENSE=$(grep "This platform has" $FILE | sed -E 's/.*has an? (.*) license/\1/')
  SN=$(grep "Serial Number" $FILE | cut -d':' -f 2 | xargs)
  KEY=$(grep "Running Activation Key" $FILE | cut -d':' -f 2 | xargs)

  echo "${HOST},${LINE},${LICENSE},${SN},${KEY}" >> $CSV

done < <(find $2 -maxdepth 1 -type f -name "*.log")
