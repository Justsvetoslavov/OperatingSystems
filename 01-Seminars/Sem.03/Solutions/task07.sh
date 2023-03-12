wc -c /etc/passwd | cut -d ' ' -f 1
wc -m /etc/passwd | cut -d ' ' -f 1
wc -l /etc/passwd | cut -d ' ' -f 1
#or
cat /etc/passwd | wc -c
cat /etc/passwd | wc -m
cat /etc/passwd | wc -l