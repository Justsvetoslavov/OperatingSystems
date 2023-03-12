 cat /etc/passwd | cut -d ':' -f 7 | grep -v '/bin/bash' | wc -l
 # or
 grep -v "/bin/bash" /etc/passwd | wc -l