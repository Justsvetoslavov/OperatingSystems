#!/bin/bash

if [[ ${#} -ne  1 ]]; then
        echo "Error: 1 argument is expected"
        exit 1
fi

if [[ ! ${1} =~ ^[a-zA-Z0-9]+$ ]]; then
        echo "String does not contain only digis and symbols"
        exit 2
fi
