cat /etc/passwd | cut -d ':' -f 1 | tr a-z A-Z
#or
cat /etc/passwd | cut -d ':' -f 1 | sed 's/./\U&/g'
