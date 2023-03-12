#!/bin/bash

if [[ ${#} -ne 1 ]]; then
        exit 1
fi

while ! who | grep -E -q "$1"; do
        echo "User is not logged."
        sleep 1
done

echo "User logged in."
exit 0
