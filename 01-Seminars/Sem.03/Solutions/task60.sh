ps -e -o user,cmd | grep "vim" | sort | uniq -c | tr -s ' ' | sed 's/ //' | awk '{if($1>=2) print $2 "\n"}'
# or 
ps -e -o user,comm | tr -s ' ' | awk '{if($2 == "vim") {print $0}}' | sort | uniq -c | head -n 1 | tr -s ' ' | cut -d ' ' -f 3