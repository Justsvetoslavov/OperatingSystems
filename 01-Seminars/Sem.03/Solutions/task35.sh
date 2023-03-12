cat /etc/services | grep -o "[a-zA-Z]*" | sort | uniq -c | sort -nr | head -n 10 | tr -s ' ' | sed "s/ //"
#or
grep -o "[a-zA-Z]*" /etc/services | sort | uniq -c | tr -s " " | sed 's/ //' | sort -nr -k 1 | head -n 10