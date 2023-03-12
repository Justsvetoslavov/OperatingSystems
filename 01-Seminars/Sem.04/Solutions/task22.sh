#!/bin/bash

counter=0

read -p "Enter interval for the number: " a b

if [[ ! $a =~ ^[0-9]+$ || ! $b =~ ^[0-9]+$ ]]; then
        echo "Argument is not a number"
        exit 1
fi

if [[ $a -gt $b ]] ; then
        echo "Invalid interval"
        exit 1
fi

randomNumber=$(( (RANDOM % b) + a ))

while true; do
        counter=$(( counter + 1 ))
        read -p "Enter your guess: " guess
        if [[ ! $guess =~ ^[0-9]+$ ]]; then
                echo "Invalid input: Please enter a number."
                continue
        fi

        if [[ ${guess} -lt ${a} ]] || [[ ${guess} -gt ${b} ]] ; then
                echo "Your guess is out of interval"
        elif [[ ${guess} -eq ${randomNumber} ]] ; then
                echo "Congrats, you guessed in ${counter} attempts"
                exit 0
        elif [[ ${guess} -lt ${randomNumber} ]] ; then
                echo "Higher"
        else
                echo "Lower"
        fi
done