cat file{1,2,3} | grep -o '.' | sort | uniq -c | sort -nr | tr -s ' ' | sed "s/ //"
# or 
grep -o "." f{1,2,3} | cut -d ':' -f 2 | sort | uniq -c | tr -s ' ' | sed 's/ //' | sort -nr -k 1