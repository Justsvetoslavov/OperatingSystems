#!/bin/bash

if [ $# -ne 3 ]; then
        echo "Arg count"
        exit 1
fi

if [ ! -f $1 ]; then
        echo "First argument must be a file"
        exit 1
fi

FIRSTKEY_VALS=$(cat $1 | grep -E "^$2=" | cut -d = -f2)
SECONDKEY_VALS=$(cat $1 | grep -E "^$3=" | cut -d = -f2)

UNIQUE=$(comm <(echo $FIRSTKEY_VALS | sed 's/ /\n/g' | sort | uniq) <(echo $SECONDKEY_VALS| sed 's/ /\n/g' | sort | uniq) -1 -3 | xargs)

sed -E -i "s/^($3=).*$/\1$UNIQUE/" $1

# or 

#!/bin/bash

test $# -eq 3 || exit 1

FILE=$1
STR1=$2
STR2=$3

KEY1=$(grep "^$STR1=" $FILE)
KEY2=$(grep "^$STR2=" $FILE)

if test -z "$KEY2"
then
        echo "$STR2=" >> $FILE
        exit 0
fi

VALUE1=$(echo $KEY1 | cut -d "=" -f 2)
VALUE2=$(echo $KEY2 | cut -d "=" -f 2)


NEW2=$(comm -13 \
        <(echo $VALUE1 | xargs | tr " " "\n" | sort) \
        <(echo $VALUE2 | xargs | tr " " "\n" | sort) \
        | xargs)

sed -i -e "s/^$STR2=$VALUE2/$STR2=$NEW2/" $FILE
