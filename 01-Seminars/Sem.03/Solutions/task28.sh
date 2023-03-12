grep -o "." /etc/passwd | egrep -v "a|а"| wc -l
#or
grep -o "." /etc/passwd | grep -E -v "a|а"| wc -l
