cat /etc/passwd | head -n 46 | tail -n 19 | cut -d ':' -f 3 | egrep -o '.$'
#or
cat /etc/passwd | head -n 46 | tail -n 19 | cut -d ':' -f 3 | rev | cut -c 1
#or
head -n 46 /etc/passwd | tail -n 19 | cut -d ':' -f 3 | cut -c 4-
#or
cat /etc/passwd | head -n 46 | tail -n 19 | cut -d ':' -f 3 | sed 's/.*\(.\)$/\1/g'
