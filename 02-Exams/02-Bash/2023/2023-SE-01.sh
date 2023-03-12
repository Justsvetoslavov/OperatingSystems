#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Arg count"
        exit 1
fi

if [ ! -e $1 ]; then
        echo "File $1 not found"
        exit 2
fi

if [ ! -d $2 ]; then
        echo "Dir $2 not found"
        exit 2
fi

WORDSFILE=$1
FILESDIR=$2

for BADWORD in $(cat $WORDSFILE); do
        REPLACE=$(echo $BADWORD | tr [a-zA-Z0-9_] "*")
        for FILE in $(find $FILESDIR -iname "*.txt"); do
                # Note how exact word is: \<${BADWORD}\>
                sed -i -E "s/\<${BADWORD}\>/${REPLACE}/g" $FILE
        done
done
