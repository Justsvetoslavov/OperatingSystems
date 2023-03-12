#!/bin/bash

if [[ $# -ne 1 ]] ; then
        echo "Expected 1 argument - username"
        exit 1
fi

pkill -u "${1}"