find / -type f -printf "%n %f\n" 2>/dev/null | sort -nr -t ' ' -k 1 | head -n 5 | cut -d ' ' -f 2
#or
# if there are more than 5 with the same number of hardlinks
find / -type f -printf "%n %f\n" 2>/dev/null | sort -nr -t ' ' -k 1 | awk -F ' ' 'BEGIN{max=0;counter=0} {if(NR==1){max=$1} if(counter < 5 || max==$1) {counter+=1; print $2}}'