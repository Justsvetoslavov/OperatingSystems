#!/bin/bash

if [[ ${#} -ne 3 ]]; then
	echo "Error: Invalid number of arguments"
	exit 1
fi

if [[ ! ${1} =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
	echo "First argument has to be a number"
	exit 2
fi

if [[ ! ${2} =~ ^[a-zA-Z]$ ]]; then
	echo "Invalid second argument ${2}"
	exit 2
fi

if [[ ! ${3} =~ ^[a-zA-Z]+$ ]]; then
	echo "Third argument has to be a letter/symbol"
	exit 2
fi

basefile="base.csv"
prefixfile="prefix.csv"

if [[ ! -f ${basefile} ]]; then
	echo "Error: base.csv is not found"
	exit 3
fi

if [[ ! -f ${prefixfile} ]]; then
	echo "Error: prefix.csv is not found"
	exit 3
fi

multiplier=$(grep -E ",${2}," "${prefixfile}" | cut -d ',' -f 3)
if [[ -z ${multiplier} ]]; then
	echo "Error: prefix symbol not found"
	exit 4
fi

value=$(echo "scale=1; ${1}*${multiplier}" | bc)

unit_name=$(grep -E ",${3}," "${basefile}" | cut -d ',' -f 1)
measure=$(grep -E "${3}" "${basefile}" | cut -d ',' -f 3)

if [[ -z ${unit_name} ]] || [[ -z ${measure} ]]; then
	echo "Error: invalid unit name or measure"
	exit 5
fi

echo "Result: ${value} ${3} (${unit_name}, ${measure})"



