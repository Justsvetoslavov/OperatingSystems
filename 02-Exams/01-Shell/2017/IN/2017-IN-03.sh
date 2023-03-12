grep "$(grep "/home/Inf" etc/passwd | cut -d ':' -f 5 | cut -d ',' -f 1 | grep -E "[a|а]$")" /etc/passwd | cut -d ':' -f 1 | cut -c 3-4 | sort | uniq -c | tr -s ' ' | sed 's/ //' | sort -k 1 -nr | head -n 1
#or
cat passwd | egrep "/home/Inf/" | awk -F ':' '{print $1 " " $5}' | awk '{print $1 $3}' | egrep ".*a[,]{0,1}$" | sed -E 's/..(..).*/\1/g' | sort -n | uniq -c | sort -nr | head -n 1
#or
# Note: Informatics here is marked as I
cat /etc/passwd | cut -d ':' -f 1,5 | tr -s ',' | tr ',' ' ' | egrep "^.*:[a-zA-Zа-яА-Я]+ [a-zA-Zа-яА-Я]+[a|а] I$" | cut -d ':' -f 1 | cut -c 2,3 | sort -nr | uniq -c | tr -s ' ' | sed 's/ //'
