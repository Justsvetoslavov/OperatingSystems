cat /etc/passwd | cut -f 1,6 -d ':'| tr ':' ' '

# or

# delete redundant comas in names from /etc/passwd as well
cat /etc/passwd | cut -d ':' -f 5,6 | tr ':' ' ' | tr -d ','
