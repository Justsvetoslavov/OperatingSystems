cat /etc/passwd | cut -d ':' -f 5 | grep -o "." | sort | uniq -c | tr -s ' ' | sed 's/ //' | sort -n -k 1 | awk '{if($1==1) print $2}' | wc -l
