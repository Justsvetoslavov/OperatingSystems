#!/bin/bash

read -p "Enter a string:" username

if ! grep -E -q ${username} /etc/passwd; then
        echo "Error: username does not exist"
        exit 1
fi

sessions=$(who | grep "${username}" | wc -l)
echo "Active sessions: ${sessions}"
